#include "SceneManager.h"
#include "Easing.h"
#include "ImGuiManager.h"
#include "DirectX.h"
#include "ImGuiController.h"
#include "InputManager.h"
#include "XAudioManager.h"
#include "LoadingSprite.h"
#include "LoadingModel.h"
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
	sceneDrawable_ = false;
}

void SceneManager::Initialize()
{
	sceneChangeCounter_.Initialize(60, true, true);

	sceneFactry_ = std::make_unique<SceneFactory>();
	scene_ = sceneFactry_->CreateScene("TITLESCENE");

	blackScreen_.Initialize();
	blackScreen_.SetSize({ Window::sWIN_WIDTH,Window::sWIN_HEIGHT });
	blackScreen_.SetColor({ 0.0f,0.0f,0.0f,alpha_ });
#pragma region Loading

	endLoading_ = true;
	loadObj_ = std::make_unique<LoadingModel>();
	loadObj_->Initialize();
	loadObj_->SetIsLoading(!endLoading_);

	dissolveSprite_ = std::make_unique<DissolveSprite>();
	dissolveSprite_->Initialize();
	dissolveSprite_->SetSize(Vector2D(Window::sWIN_WIDTH, Window::sWIN_HEIGHT));
	dissolveSprite_->SetColor(Vector4D(0.8f, 0.8f, 0.8f, 1.0f));

	TextureManager::GetInstance()->LoadTextureGraph("noise.png");

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

	ParticleManager::GetInstance()->Initialize();

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

void SceneManager::ScreenColorUpdate()
{
	sceneChangeCounter_.Update();

	if (sceneChangeCounter_.GetIsActive()) {
		//float color = Easing::lerp(1.0f, 0.0f, sceneChangeCounter_.GetCountPerMaxCount());
		//screenColor_.x = color;
		//screenColor_.y = color;
		//screenColor_.z = color;

		////	色設定
		//mainScene->SetColor(screenColor_);
	}
	float value = Easing::EaseOut(0.0f, 1.0f, sceneChangeCounter_.GetCountPerMaxCount(), 2);
	SetDissolveValue(value);
}

void SceneManager::SplashUpdate()
{
	//	スプラッシュスクリーンじゃなかったら
	if (!isSplashScreen_) return;
	
	splashScene_->Update();

	//	スプラッシュスクリーン終わり
	if (splashScene_->EndScene()) {
		//	ローディング表示
		loadObj_->SetIsLoading(true);

		//	次のシーン読み込み終了
		std::future_status loadStatus = sceneInitInfo_.wait_for(std::chrono::seconds(0));
		if (loadStatus == std::future_status::ready /*&& InputManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_SPACE)*/) {

			isSplashScreen_ = false;
			scene_->FirstFrameUpdate();
			loadObj_->SetIsLoading(false);
			sceneDrawable_ = true;

			splashScene_->Finalize();
			splashScene_.release();

			sceneChangeCounter_.SetIsIncrement(false);
			sceneChangeCounter_.StartCount();

			//screenColor_ = { 0.0f,0.0f,0.0f,1.0f };
			////	色設定
			//mainScene->SetColor(screenColor_);
			SetDissolveValue(1.0f);
		}
	}
}

void SceneManager::SceneFadeInUpdate()
{
	if (!endLoading_)		return;
	if (!sceneInitialized_)	return;

	////	フェードイン済み
	//bool fadedIn = sceneChangeCounter_.GetFrameCount() == sceneChangeCounter_.GetMaxFrameCount();
	//if (fadedIn) {
	//	//	同期処理
	//}
	scene_->Update();
}

void SceneManager::SceneFadeOutUpdate()
{
	if (!endLoading_)		return;
	if (sceneInitialized_)	return;

	//	フェードアウト済み
	bool fadedOut = sceneChangeCounter_.GetFrameCount() == sceneChangeCounter_.GetMaxFrameCount();
	if (fadedOut) {
		sceneDrawable_ = false;
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
		sceneDrawable_ = true;
		//	ロード終わりフラグ
		endLoading_ = true;
		loadObj_->SetIsLoading(!endLoading_);
		//	firstFrame(音再生タイミング)
		scene_->FirstFrameUpdate();

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

	imguiMan->Text("sceneInitialized : %d", sceneInitialized_);
	imguiMan->Text("sceneDrawable : %d", sceneDrawable_);

	ImGuiManager::GetInstance()->End();

#endif // _DEBUG
}

void SceneManager::Update()
{
	CameraManager::GetInstance()->Update();

	ScreenColorUpdate();

	SplashUpdate();

	SceneUpdate();

	loadObj_->Update();
	dissolveSprite_->Update();

	blackScreen_.Update();

	ImguiUpdate();
}

void SceneManager::Draw()
{
	MyDirectX* dx = MyDirectX::GetInstance();

#pragma region DrawScreen
	Vector4D shadowClearColor_(1.0f, 1.0f, 1.0f, 1.0f);

	dx->PrevPostEffect(shadowEffect.get(), shadowClearColor_);

	if (sceneDrawable_ && !isSplashScreen_) {
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
	if (sceneDrawable_) {
		scene_->Draw();
		ModelManager::GetInstance()->DrawPreview();
		CameraManager::GetInstance()->DrawTarget();
		UIEditor::GetInstance()->Draw();

		TextureManager::GetInstance()->DrawPreview();
		if (alpha_ != 0.0f) {
			blackScreen_.Draw();
		}
		GameOverUI::GetInstance()->Draw();
	}

	dx->PostEffectDraw(mainScene.get());

	Vector4D luminnceClearColor_(0.0f, 0.0f, 0.0f, 1.0f);
	dx->PrevPostEffect(luminnce.get(), luminnceClearColor_);

	mainScene->DrawLuminnce();

	dx->PostEffectDraw(luminnce.get());

	luminnceBulr->Draw();

	dx->PrevPostEffect(glayscale.get());

	//if (!isSplashScreen_) {
		PostEffect* main = mainScene.get();

		main->Draw(PipelineManager::GetInstance()->GetPipeline("PostEffect"), false, luminnceBulr->GetTexture(0)->GetHandle());
	//}

	dx->PostEffectDraw(glayscale.get());


#pragma region DrawBackBuffer
	dx->PrevDraw();

	glayscale->DrawGlay();

	dissolveSprite_->Draw();

	loadObj_->Draw();

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
	if (alpha_ == alpha) return;
	alpha_ = alpha;
	blackScreen_.SetColor({ 0.0f,0.0f,0.0f,alpha_ });
}

void SceneManager::SceneChange()
{
	if (nextScene_ != nullptr) {
		if (scene_ != nullptr) {
			scene_->Finalize();
		}

		scene_.reset(nextScene_.get());
		SceneInitialize();
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
