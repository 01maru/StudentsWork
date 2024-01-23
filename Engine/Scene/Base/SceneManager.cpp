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

using namespace MNE;

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

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

	mainScene = std::make_unique<PostEffect>();
	mainScene->Initialize(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, "main", 2, DXGI_FORMAT_R11G11B10_FLOAT);

	glayscale = std::make_unique<GlayScale>();
	glayscale->Initialize(mainScene.get());

	luminnce = std::make_unique<PostEffect>();
	luminnce->Initialize(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, "luminnce", 2, DXGI_FORMAT_R11G11B10_FLOAT);

	luminnceBulr = std::make_unique<GaussBlur>();
	luminnceBulr->Initialize(5.0f, luminnce.get(), DXGI_FORMAT_R11G11B10_FLOAT);
	luminnceBulr->SetPipeline(PipelineManager::GetInstance()->GetPipeline("luminncexBlur"),
		PipelineManager::GetInstance()->GetPipeline("luminnceyBlur"));

	shadowEffect = std::make_unique<PostEffect>();
	shadowEffect->Initialize(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, "shadow", 2, DXGI_FORMAT_R32G32_FLOAT);

	shadowBulr = std::make_unique<GaussBlur>();
	shadowBulr->Initialize(1.0f, shadowEffect.get(), DXGI_FORMAT_R32G32_FLOAT);
	shadowBulr->SetPipeline(PipelineManager::GetInstance()->GetPipeline("xBlur"),
		PipelineManager::GetInstance()->GetPipeline("yBlur"));
	shadowBulr->SetClearColor({ 1.0f,1.0f,1.0f,1.0f });

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

void SceneManager::Finalize()
{
	scene_->Finalize();

#ifdef _DEBUG

	ImGuiManager::GetInstance()->Finalize();

#endif // _DEBUG
}

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

void SceneManager::ImguiUpdate()
{
#ifdef _DEBUG
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	ImGuiManager::GetInstance()->Begin();
	ImGuiController::GetInstance()->Update();

	InputManager::GetInstance()->ImGuiUpdate();
	UIEditor::GetInstance()->ImGuiUpdate();
	glayscale->SetGlayScale(UIEditor::GetInstance()->GetActiveGlayscale());
	CameraManager::GetInstance()->ImGuiUpdate();
	XAudioManager::GetInstance()->ImguiUpdate(endLoading_);
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

	ImguiUpdate();
}

void SceneManager::Draw()
{
	MyDirectX* dx = MyDirectX::GetInstance();

#pragma region DrawScreen
	Vector4D shadowClearColor_(1.0f, 1.0f, 1.0f, 1.0f);

	dx->PrevPostEffect(shadowEffect.get(), shadowClearColor_);

	if (endLoading_ && !isSplashScreen_) {
		drawShadow_ = true;
		scene_->DrawShadow();
	}
	
	dx->PostEffectDraw(shadowEffect.get());


	//shadowBulr->Draw();

	
	dx->PrevPostEffect(mainScene.get());

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

	dx->PostEffectDraw(mainScene.get());

	//Vector4D luminnceClearColor_(0.0f, 0.0f, 0.0f, 1.0f);
	//dx->PrevPostEffect(luminnce.get(), luminnceClearColor_);

	//mainScene->DrawLuminnce();

	//dx->PostEffectDraw(luminnce.get());

	//luminnceBulr->Draw();

	dx->PrevPostEffect(glayscale.get());

	PostEffect* main = mainScene.get();
	main->Draw(PipelineManager::GetInstance()->GetPipeline("PostEffect"), false, luminnceBulr->GetTexture(0)->GetHandle());

	dx->PostEffectDraw(glayscale.get());


#pragma region DrawBackBuffer
	dx->PrevDraw();

	glayscale->DrawGlay();

	loading_.Draw();

#ifdef _DEBUG
	ImGuiManager::GetInstance()->Draw();
#endif // _DEBUG

	dx->PostDraw();
#pragma endregion
#pragma endregion

	dx->DrawEnd();

	if (endLoading_) TextureManager::GetInstance()->UploadTexture();
}

void SceneManager::ChangeScreenAlpha(float alpha)
{
	blackScreen_.SetAlphaColor(alpha);
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
