#include "SceneManager.h"
#include "Easing.h"
#include "ImGuiManager.h"
#include "DirectX.h"
#include "ImGuiController.h"
#include "InputManager.h"
#include "XAudioManager.h"
#include "LoadingSprite.h"
#include "SceneFactory.h"
#include "TextureManager.h"
#include "UIManager.h"
#include "CameraManager.h"
#include "Light.h"

#include "Window.h"

#include "PipelineManager.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

void SceneManager::FirstSceneInitialize()
{
	scene_->Initialize();
	//	画像転送
	TextureManager::GetInstance()->UploadTexture();
}

void SceneManager::SplashScreenInitialize()
{
	//	非同期
	sceneInitInfo_ = std::async(std::launch::async, [this] {return FirstSceneInitialize(); });
	isSplashScreen_ = true;

	splashSprite_ = std::make_unique<SplashSprite>();
	splashSprite_->Initialize();

	sceneChangeCounter_.SetIsIncrement(true);
	sceneChangeCounter_.StartCount();

	//	画像転送
	TextureManager::GetInstance()->UploadTexture();
}

void SceneManager::Initialize()
{
	sceneChangeCounter_.Initialize(60, true, true);

	sceneFactry_ = std::make_unique<SceneFactory>();
	scene_ = sceneFactry_->CreateScene("GAMESCENE");

#pragma region Loading

	endLoading_ = true;
	loadObj_ = std::make_unique<LoadingSprite>();
	loadObj_->Initialize();
	loadObj_->SetIsLoading(!endLoading_);

#pragma endregion

#pragma region PostEffect

	mainScene = std::make_unique<PostEffect>();
	mainScene->Initialize(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, DXGI_FORMAT_R11G11B10_FLOAT);

	dofscene = std::make_unique<PostEffect>();
	dofscene->Initialize(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, 2, DXGI_FORMAT_R11G11B10_FLOAT);

	taskScene = std::make_unique<PostEffect>();
	taskScene->Initialize(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, DXGI_FORMAT_R11G11B10_FLOAT);

	glayscale = std::make_unique<GlayScale>();
	glayscale->Initialize(mainScene.get());

	luminnce = std::make_unique<PostEffect>();
	luminnce->Initialize(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, DXGI_FORMAT_R11G11B10_FLOAT);

	luminnceBulr = std::make_unique<GaussBlur>();
	luminnceBulr->Initialize(5.0f, luminnce.get(), DXGI_FORMAT_R11G11B10_FLOAT);
	luminnceBulr->SetPipeline(PipelineManager::GetInstance()->GetPipeline("luminncexBlur"),
		PipelineManager::GetInstance()->GetPipeline("luminnceyBlur"));

	shadowEffect = std::make_unique<PostEffect>();
	shadowEffect->Initialize(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, DXGI_FORMAT_R32G32_FLOAT);

	shadowBulr = std::make_unique<GaussBlur>();
	shadowBulr->Initialize(1.0f, shadowEffect.get(), DXGI_FORMAT_R32G32_FLOAT);
	shadowBulr->SetPipeline(PipelineManager::GetInstance()->GetPipeline("xBlur"),
		PipelineManager::GetInstance()->GetPipeline("yBlur"));
	shadowBulr->SetClearColor({ 1.0f,1.0f,1.0f,1.0f });

#pragma endregion

#ifdef _DEBUG
	ImGuiManager::GetInstance()->Initialize();
#endif // _DEBUG

#pragma region SplashScreen

	//	releaseだったらスプラッシュスクリーンあり
#ifdef NDEBUG
	SplashScreenInitialize();
#endif // NDEBUG


	//	debugだったらスプラッシュスクリーンなし
#ifdef _DEBUG
	FirstSceneInitialize();
#endif // _DEBUG

#pragma endregion
}

void SceneManager::Finalize()
{
	scene_->Finalize();

#ifdef _DEBUG

	ImGuiManager::GetInstance()->Finalize();

#endif // _DEBUG
}

void SceneManager::ScreenColorUpdate()
{
	sceneChangeCounter_.Update();

	if (sceneChangeCounter_.GetIsActive()) {
		float color = Easing::lerp(1.0f, 0.0f, sceneChangeCounter_.GetCountPerMaxCount());
		screenColor_.x = color;
		screenColor_.y = color;
		screenColor_.z = color;

		//	色設定
		mainScene->SetColor(screenColor_);
	}
}

void SceneManager::SplashUpdate()
{
#ifdef NDEBUG

	//	スプラッシュスクリーンじゃなかったら
	if (!isSplashScreen_) return;

	if (sceneChangeCounter_.GetFrameCount() == sceneChangeCounter_.GetMaxFrameCount()) splashSprite_->StartCounter();
	
	splashSprite_->Update();

	std::future_status loadStatus = sceneInitInfo_.wait_for(std::chrono::seconds(0));
	if (loadStatus == std::future_status::ready && splashSprite_->SplashEnd()) {

		//	スプラッシュスクリーン終わり
		isSplashScreen_ = false;
		scene_->Update();

		splashSprite_->Finalize();

		sceneChangeCounter_.SetIsIncrement(false);
		sceneChangeCounter_.StartCount();

		screenColor_ = { 0.0f,0.0f,0.0f,1.0f };
		//	色設定
		mainScene->SetColor(screenColor_);
	}

#endif // NDEBUG
}

void SceneManager::SceneFadeInUpdate()
{
	if (!endLoading_)		return;
	if (!sceneInitialized_)	return;

	//	フェードイン済み
	bool fadedIn = sceneChangeCounter_.GetFrameCount() == 0;
	if (fadedIn) {
		//	同期処理
		scene_->Update();
	}
}

void SceneManager::SceneFadeOutUpdate()
{
	if (!endLoading_)		return;
	if (sceneInitialized_)	return;

	//	フェードアウト済み
	bool fadedOut = sceneChangeCounter_.GetFrameCount() == sceneChangeCounter_.GetMaxFrameCount();
	if (fadedOut) {
		//	フェードアウト済みロード画面へ
		endLoading_ = false;
		//	非同期
		sceneInitInfo_ = std::async(std::launch::async, [this] {return SceneChange(); });
	}
}

void SceneManager::SceneAsyncUpdate()
{
	//	ロード中じゃなかったら
	if (endLoading_) return;

	std::future_status loadStatus = sceneInitInfo_.wait_for(std::chrono::seconds(0));
	if (loadStatus == std::future_status::ready) {
		//	ロード終わりフラグ
		endLoading_ = true;
		loadObj_->SetIsLoading(!endLoading_);
		//	フェードイン
		sceneChangeCounter_.SetIsIncrement(false);
		sceneChangeCounter_.StartCount();
	}
}

void SceneManager::SceneUpdate()
{
	//	スプラッシュスクリーンだったら
	if (isSplashScreen_) return;

	SceneFadeInUpdate();
	SceneFadeOutUpdate();
	
	SceneAsyncUpdate();
}

void SceneManager::ImguiUpdate()
{
#ifdef _DEBUG

	ImGuiManager::GetInstance()->Begin();
	ImGuiController::GetInstance()->Update();

	InputManager::GetInstance()->ImGuiUpdate();
	UIManager::GetInstance()->ImGuiUpdate();
	glayscale->SetGlayScale(UIManager::GetInstance()->GetActiveGlayscale());
	CameraManager::GetInstance()->ImGuiUpdate();
	XAudioManager::GetInstance()->ImguiUpdate(endLoading_);
	TextureManager::GetInstance()->ImGuiUpdate();
	Light::GetInstance()->ImGuiUpdate();

	if (endLoading_) {
		scene_->ImguiUpdate();
	}
	ImGuiManager::GetInstance()->End();

#endif // _DEBUG
}

void SceneManager::Update()
{
	ScreenColorUpdate();

	SplashUpdate();

	SceneUpdate();

	loadObj_->Update();

	ImguiUpdate();
}

void SceneManager::Draw()
{
	MyDirectX* dx = MyDirectX::GetInstance();

#pragma region DrawScreen
	Vector4D shadowClearColor_(1.0f, 1.0f, 1.0f, 1.0f);

	dx->PrevPostEffect(shadowEffect.get(), shadowClearColor_);

	if (endLoading_ && !isSplashScreen_) {
		scene_->DrawShadow();
	}
	
	dx->PostEffectDraw(shadowEffect.get());


	shadowBulr->Draw();

	
	dx->PrevPostEffect(mainScene.get());

	if (endLoading_ && !isSplashScreen_) {
		scene_->Draw();
		UIManager::GetInstance()->Draw();
	}

	dx->PostEffectDraw(mainScene.get());
	
	if (ImGuiController::GetInstance()->GetActiveDof() ||
		ImGuiController::GetInstance()->GetActiveTask()) {
		dx->PrevPostEffect(dofscene.get());

		if(ImGuiController::GetInstance()->GetActiveTask())	mainScene->DrawTask();
		else												mainScene->DrawDoF();

		dx->PostEffectDraw(dofscene.get());
	}
	
	if (ImGuiController::GetInstance()->GetActiveTask()) {
		dx->PrevPostEffect(taskScene.get());

		dofscene->DrawMultiTask();

		dx->PostEffectDraw(taskScene.get());
	}


	Vector4D luminnceClearColor_(0.0f, 0.0f, 0.0f, 1.0f);
	dx->PrevPostEffect(luminnce.get(), luminnceClearColor_);

	mainScene->DrawLuminnce();

	dx->PostEffectDraw(luminnce.get());

	luminnceBulr->Draw();

	dx->PrevPostEffect(glayscale.get());

	if (!isSplashScreen_) {
		PostEffect* main = mainScene.get();
		if (ImGuiController::GetInstance()->GetActiveDof()) main = dofscene.get();
		if (ImGuiController::GetInstance()->GetActiveTask()) main = taskScene.get();

		main->Draw(PipelineManager::GetInstance()->GetPipeline("PostEffect"), false, luminnceBulr->GetTexture(0)->GetHandle());
	}

	dx->PostEffectDraw(glayscale.get());


#pragma region DrawBackBuffer
	dx->PrevDraw();

	glayscale->DrawGlay();

	loadObj_->Draw();

#ifdef NDEBUG

	if (isSplashScreen_) {
		splashSprite_->Draw();
	}

#endif // NDEBUG

#ifdef _DEBUG
	ImGuiManager::GetInstance()->Draw();
#endif // _DEBUG

	dx->PostDraw();
#pragma endregion
#pragma endregion

	dx->DrawEnd();
}

void SceneManager::SceneChange()
{
	if (nextScene_ != nullptr) {
		if (scene_ != nullptr) {
			scene_->Finalize();
		}

		scene_.reset(nextScene_.get());
		scene_->Initialize();
		//	画像転送
		TextureManager::GetInstance()->UploadTexture();
		sceneInitialized_ = true;
		nextScene_.release();
	}
}

void SceneManager::SetNextScene(const std::string& sceneName)
{
	nextScene_ = sceneFactry_->CreateScene(sceneName);
	
	//	nextSceneがセットされたら
	if (nextScene_ != nullptr) {
		sceneInitialized_ = false;
		loadObj_->SetIsLoading(true);

		sceneChangeCounter_.SetIsIncrement(true);
		sceneChangeCounter_.StartCount();
	}
}
