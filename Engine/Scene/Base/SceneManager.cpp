#include "SceneManager.h"
#include "SceneFactory.h"
#include "DirectX.h"
#include "Window.h"

#include "ImGuiManager.h"
#include "ImGuiController.h"
#include "InputManager.h"
#include "UIEditor.h"
#include "CameraManager.h"
#include "XAudioManager.h"
#include "TextureManager.h"
#include "LightManager.h"
#include "ParticleManager.h"
#include "ModelManager.h"
#include "PostEffectManager.h"
#include "PipelineManager.h"

#include "GrayScale.h"
#include "ShadowPostEffect.h"
#include "MainPostEffect.h"
#include "GaussBlur.h"

using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void SceneManager::SceneInitialize()
{
	scene_->Initialize();
	//	画像転送
	TextureManager::GetInstance()->AsyncUploadTexture();
}

void SceneManager::FirstScreenInitialize()
{
	//	スプラッシュスクリーンがナシなら
	if (!isSplashScreen_) {
		SceneInitialize();
		scene_->FirstFrameUpdate();
		return;
	}

	splashScene_ = std::make_unique<SplashScreenScene>();
	splashScene_->Initialize();

	//	画像転送
	TextureManager::GetInstance()->UploadTexture();

	//	非同期
	sceneInitInfo_ = std::async(std::launch::async, [this] {return SceneInitialize(); });
	//	非同期ロードに変更
	endLoading_ = false;
}

void SceneManager::Initialize()
{
	sceneFactory_ = std::make_unique<SceneFactory>();
	scene_ = sceneFactory_->CreateScene("TITLESCENE");

#pragma region Loading

	endLoading_ = true;
	loading_.Initialize();

#pragma endregion

#pragma region PostEffect

	PostEffectManager* peMan = PostEffectManager::GetInstance();
	std::unique_ptr<IPostEffect> postEffect;

	//	ShadowMap
	Vector4D clearColor(1.0f, 1.0f, 1.0f, 1.0f);
	std::unique_ptr<ShadowPostEffect> shadowMap = std::make_unique<ShadowPostEffect>();
	shadowMap->Initialize(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, "shadow", 2, DXGI_FORMAT_R32G32_FLOAT);
	shadowMap->SetClearColor(clearColor);
	postEffect = std::move(shadowMap);
	/*IPostEffect* shadowPtr = */peMan->AddPostEffectBack(postEffect);

	////	ShadowGaussBlur
	//std::unique_ptr<GaussBlur> gaussBlur = std::make_unique<GaussBlur>();
	//gaussBlur->Initialize(shadowPtr);
	//gaussBlur->SetClearColor(clearColor);
	//gaussBlur->SetWeight(1.0f);
	//GaussBlur* shadowBlur = peMan->AddGaussBlur(gaussBlur);
	//Object3D::SetShadowMapTex(shadowBlur->GetBlurredTexture());

	//	Main
	clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	std::unique_ptr<MainPostEffect> main = std::make_unique<MainPostEffect>();
	main->Initialize(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, "main", 2, DXGI_FORMAT_R11G11B10_FLOAT);
	main->SetClearColor(clearColor);
	postEffect = std::move(main);
	IPostEffect* mainPtr = peMan->AddPostEffectBack(postEffect);

	//	Luminance
	postEffect = std::make_unique<IPostEffect>();
	postEffect->Initialize(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, "luminance", 2, DXGI_FORMAT_R11G11B10_FLOAT);
	postEffect->SetOriginalPostEffect(mainPtr);
	postEffect->SetClearColor(clearColor);
	postEffect->SetMode(MainPostEffect::Luminance);

	IPostEffect* luminancePtr = peMan->AddPostEffectBack(postEffect);

	//	LuminanceGaussBlur
	std::unique_ptr<GaussBlur> gaussBlur = std::make_unique<GaussBlur>();
	gaussBlur->Initialize(luminancePtr);
	gaussBlur->SetClearColor(clearColor);
	gaussBlur->SetWeight(5.0f);
	GaussBlur* luminanceBlur = peMan->AddGaussBlur(gaussBlur);

	MainPostEffect* mainPE = dynamic_cast<MainPostEffect*>(mainPtr);
	mainPE->SetLuminanceTex(0, luminanceBlur->GetBlurredTexture());

	//	GrayScale
	std::unique_ptr<GrayScale> gray = std::make_unique<GrayScale>();
	gray->Initialize(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, "Gray", 2, DXGI_FORMAT_R11G11B10_FLOAT);
	gray->SetOriginalPostEffect(mainPtr);
	postEffect = std::move(gray);
	IPostEffect* grayPtr = peMan->AddPostEffectBack(postEffect);
	UIEditor::GetInstance()->SetGrayScalePE(dynamic_cast<GrayScale*>(grayPtr));
	peMan->SetBackBuffer(grayPtr);

#pragma endregion

#ifdef _DEBUG
	ImGuiManager::GetInstance()->Initialize();
#endif // _DEBUG

	//	releaseだったらスプラッシュスクリーンあり
#ifdef NDEBUG
	isSplashScreen_ = true;
#endif // NDEBUG

	FirstScreenInitialize();

	ModelManager::GetInstance()->Initialize();
}

//-----------------------------------------------------------------------------
// [SECTION] Finalize
//-----------------------------------------------------------------------------

void SceneManager::Finalize()
{
	scene_->Finalize();

#ifdef _DEBUG

	ImGuiManager::GetInstance()->Finalize();

#endif // _DEBUG
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void SceneManager::SplashUpdate()
{
	//	スプラッシュスクリーンじゃなかったら
	if (!isSplashScreen_) return;
	
	splashScene_->Update();

	//	スプラッシュスクリーン終わり
	if (splashScene_->EndScene()) {
		//	ローディング表示
		loading_.SetIsLoading(true);

		//	次のシーン読み込み終了
		std::future_status loadStatus = sceneInitInfo_.wait_for(std::chrono::seconds(0));
		if (loadStatus == std::future_status::ready) {

			isSplashScreen_ = false;
			scene_->FirstFrameUpdate();
			loading_.SetIsLoading(false);
			//	非同期ロードに終了
			endLoading_ = true;
			splashScene_->Finalize();
			splashScene_.release();
		}
	}
}

void SceneManager::SceneUpdate()
{
	//	ロード終わっていなかったら更新しない
	if (endLoading_ == false)	return;

	scene_->Update();
}

void SceneManager::SceneAsyncInitialize()
{
	//	ロード終わっていなかったら初期化しない
	if (endLoading_ == false)			return;

	//	ロード画面描画し終わってなかったら初期化しない
	if (loading_.GetIsDrawn() == false) return;

	ParticleManager::GetInstance()->DeleteAllParticle();

	//	非同期
	sceneInitInfo_ = std::async(std::launch::async, [this] {return SceneChange(); });
	//	非同期ロードに変更
	endLoading_ = false;
}

void SceneManager::SceneAsyncUpdate()
{
	//	ロード中じゃなかったら更新しない
	if (endLoading_ == true) return;

	std::future_status loadStatus = sceneInitInfo_.wait_for(std::chrono::seconds(0));
	if (loadStatus == std::future_status::ready) {
		//	ロード終わり
		endLoading_ = true;
		loading_.SetIsLoading(!endLoading_);
		//	firstFrame(音再生タイミング)
		scene_->FirstFrameUpdate();

		//	フェードアウト
		loading_.StartFadeAnimation(false);
	}
}

void SceneManager::AllSceneUpdate()
{
	//	スプラッシュスクリーンだったら
	if (isSplashScreen_) return;

	SceneUpdate();
	SceneAsyncInitialize();
	
	SceneAsyncUpdate();
}

//-----------------------------------------------------------------------------
// [SECTION] ImGuiUpdate
//-----------------------------------------------------------------------------

void MNE::SceneManager::SceneManagerImGuiUpdate()
{
	if (ImGuiController::GetInstance()->GetActiveSceneManager() == FALSE) return;

	ImGuiManager* imGui = ImGuiManager::GetInstance();

	imGui->BeginWindow("SceneManager");

	imGui->Text("endLoading : %d", endLoading_);

	imGui->EndWindow();
}

void SceneManager::ImGuiUpdate()
{
#ifdef _DEBUG
	ImGuiManager* imGuiMan = ImGuiManager::GetInstance();

	//	デバッグモード切替
	if (InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_RETURN, InputJoypad::BACK_Button))
	{
		debugging_ = !debugging_;
	}

	if (debugging_ == TRUE)
	{
		imGuiMan->Begin();
		ImGuiController::GetInstance()->Update();

		InputManager::GetInstance()->ImGuiUpdate();
		UIEditor::GetInstance()->ImGuiUpdate();
		CameraManager::GetInstance()->ImGuiUpdate();
		XAudioManager::GetInstance()->ImGuiUpdate(endLoading_);
		TextureManager::GetInstance()->ImGuiUpdate();
		LightManager::GetInstance()->ImGuiUpdate();
		ParticleManager::GetInstance()->ImGuiUpdate();
		ModelManager::GetInstance()->ImGuiUpdate();
		PostEffectManager::GetInstance()->ImGuiUpdate();
		PipelineManager::GetInstance()->ImGuiUpdate();

		if (endLoading_) {
			scene_->ImguiUpdate();
		}

		SceneManagerImGuiUpdate();

		imGuiMan->End();
	}

#endif // _DEBUG
}

void SceneManager::Update()
{
	CameraManager::GetInstance()->Update();

	SplashUpdate();

	AllSceneUpdate();

	//	ロード画面
	loading_.Update();

	InputManager::GetInstance()->MatUpdate();

	ImGuiUpdate();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void MNE::SceneManager::DrawShadow()
{
	if (endLoading_ && !isSplashScreen_) {
		drawShadow_ = true;
		scene_->DrawShadow();
	}
}

void MNE::SceneManager::DrawScene()
{
	if (isSplashScreen_) {
		splashScene_->Draw();
	}
	else {
	}
	if (endLoading_) {
		drawShadow_ = false;
		scene_->Draw();
		ModelManager::GetInstance()->DrawPreview();
		CameraManager::GetInstance()->DrawTarget();

		scene_->DrawUIBeforeBlackScreen();

		scene_->DrawUIAfterBlackScreen();
		InputManager::GetInstance()->Draw();

		UIEditor::GetInstance()->Draw();
		TextureManager::GetInstance()->DrawPreview();
	}
}

void MNE::SceneManager::DrawBackBuffer()
{
	MyDirectX* dx = MyDirectX::GetInstance();
	PostEffectManager* peMan = PostEffectManager::GetInstance();

	dx->PrevDraw(peMan->GetBackBufferPtr()->GetClearColor());

	//	最後の描画
	peMan->DrawBackBuffer();

	//	ロード画面
	loading_.Draw();

	//	ImGui表示
#ifdef _DEBUG
	if (debugging_ == TRUE)
	{
		ImGuiManager::GetInstance()->Draw();
	}
#endif // _DEBUG

	dx->PostDraw();
}

void SceneManager::Draw()
{
	//	ポストエフェクト更新
	PostEffectManager::GetInstance()->Update();

	//	バックバッファに描画
	DrawBackBuffer();

	//	バックとフロントバッファの切り替え
	MyDirectX::GetInstance()->DrawEnd();

	if (endLoading_) TextureManager::GetInstance()->UploadTexture();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

bool MNE::SceneManager::GetIsDrawShadow()
{
	return drawShadow_;
}

bool MNE::SceneManager::GetGameLoop()
{
	return gameLoop_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void SceneManager::SceneChange()
{
	//	次のシーンがあったら切り替える
	if (nextScene_ != nullptr) {
		//	既にシーンがあったら終了処理
		if (scene_ != nullptr) {
			scene_->Finalize();
		}

		scene_ = std::move(nextScene_);
		SceneInitialize();
	}
}

void MNE::SceneManager::GameLoopEnd()
{
	gameLoop_ = FALSE;
}

void SceneManager::SetNextScene(const std::string& sceneName)
{
	nextScene_ = sceneFactory_->CreateScene(sceneName);
	
	//	nextSceneがセットされたら
	if (nextScene_ != nullptr) {
		loading_.SetIsLoading(TRUE);

		//	フェードイン
		loading_.StartFadeAnimation(TRUE);
	}
}
