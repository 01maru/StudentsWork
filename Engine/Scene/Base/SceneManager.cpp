#include "SceneManager.h"
#include "ImGuiManager.h"
#include "DirectX.h"
#include "ImGuiController.h"
#include "InputManager.h"
#include "XAudioManager.h"
#include "SceneFactory.h"
#include "TextureManager.h"
#include "UIEditor.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "ParticleManager.h"

#include "Window.h"

#include "ModelManager.h"
#include "PipelineManager.h"

#include "GameOverUI.h"

#include "GrayScale.h"

#include "PostEffectManager.h"
#include "ShadowPostEffect.h"
#include "MainPostEffect.h"
#include "GaussBlur.h"

using namespace MNE;
using namespace MyMath;

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

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
	sceneFactry_ = std::make_unique<SceneFactory>();
	scene_ = sceneFactry_->CreateScene("TITLESCENE");

	blackScreen_.Initialize();
	blackScreen_.SetSize({ Window::sWIN_WIDTH,Window::sWIN_HEIGHT });
	Vector3D blackColor;
	blackScreen_.SetColor(blackColor);
	blackScreen_.SetAlphaColor(0.0f);
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
	//postEffect->SetGPipeline(PipelineManager::GetInstance()->GetPipeline("MainPostEffect"));

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

void SceneManager::ImGuiUpdate()
{
#ifdef _DEBUG
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	ImGuiManager::GetInstance()->Begin();
	ImGuiController::GetInstance()->Update();

	InputManager::GetInstance()->ImGuiUpdate();
	UIEditor::GetInstance()->ImGuiUpdate();
	CameraManager::GetInstance()->ImGuiUpdate();
	XAudioManager::GetInstance()->ImGuiUpdate(endLoading_);
	TextureManager::GetInstance()->ImGuiUpdate();
	LightManager::GetInstance()->ImGuiUpdate();
	ParticleManager::GetInstance()->ImGuiUpdate();
	ModelManager::GetInstance()->ImGuiUpdate();

	if (endLoading_) {
		scene_->ImguiUpdate();
	}

	imguiMan->Text("endLoading : %d", endLoading_);

	ImGuiManager::GetInstance()->End();

#endif // _DEBUG
}

void SceneManager::Update()
{
	CameraManager::GetInstance()->Update();

	SplashUpdate();

	AllSceneUpdate();

	//	ロード画面
	loading_.Update();
	//	黒スクリーン
	blackScreen_.Update();

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

		blackScreen_.Draw();

		scene_->DrawUIAfterBlackScreen();
		InputManager::GetInstance()->Draw();

		UIEditor::GetInstance()->Draw();
		TextureManager::GetInstance()->DrawPreview();
	}
}

void MNE::SceneManager::DrawBackBuffer()
{
	MyDirectX* dx = MyDirectX::GetInstance();

	dx->PrevDraw();

	//	最後の描画
	PostEffectManager::GetInstance()->DrawBackBuffer();

	loading_.Draw();

#ifdef _DEBUG
	ImGuiManager::GetInstance()->Draw();
#endif // _DEBUG

	dx->PostDraw();
}

void SceneManager::Draw()
{
#pragma region DrawScreen

	PostEffectManager::GetInstance()->Update();

#pragma region DrawBackBuffer

	DrawBackBuffer();

#pragma endregion
#pragma endregion

	MyDirectX::GetInstance()->DrawEnd();

	if (endLoading_) TextureManager::GetInstance()->UploadTexture();
}

void SceneManager::SceneChange()
{
	if (nextScene_ != nullptr) {
		if (scene_ != nullptr) {
			scene_->Finalize();
		}

		scene_.reset(nextScene_.get());
		SceneInitialize();
		nextScene_.release();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

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

void MNE::SceneManager::GameLoopEnd()
{
	gameLoop_ = false;
}

void SceneManager::ChangeScreenAlpha(float alpha)
{
	blackScreen_.SetAlphaColor(alpha);
}

void SceneManager::SetNextScene(const std::string& sceneName)
{
	nextScene_ = sceneFactry_->CreateScene(sceneName);
	
	//	nextSceneがセットされたら
	if (nextScene_ != nullptr) {
		loading_.SetIsLoading(true);

		//	フェードイン
		loading_.StartFadeAnimation(true);
	}
}
