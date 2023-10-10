#include "TitleScene.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "XAudioManager.h"
#include "Window.h"

#include "ImGuiManager.h"
#include "CameraManager.h"
#include "TitleCamera.h"
#include "ModelManager.h"
#include "Easing.h"
#include "LightManager.h"

void TitleScene::LoadResources()
{
#pragma region Sound
	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("cursorMove.wav");
	xAudioMan->LoadSoundWave("decision.wav");
	//	タイトル音
	xAudioMan->LoadSoundWave("title.wav");
#pragma endregion

#pragma region Model
	ModelManager* models = ModelManager::GetInstance();
	models->LoadModel("grass");
	models->LoadModel("midTree");
	models->LoadModel("lowTree");
	models->LoadModel("ground");
	models->LoadModel("skydome");

	//	ステージ読み込み
	level_ = std::make_unique<JSONLoader>();
	level_->LoadJSON("Title");
	//level_->LoadJSON("test");
	//	天球
	skydome_.reset(Object3D::Create(models->GetModel("skydome")));
	//	地面
	ground_.reset(Object3D::Create(models->GetModel("ground")));
#pragma endregion
	
#pragma region UI

	uiDrawer_ = std::make_unique<UIDrawer>();
	uiDrawer_->LoadSprites("TitleScene");
	uiDrawer_->SetUI("Title");

	selectCursor_ = std::make_unique<Sprite>();
	selectCursor_->Initialize(TextureManager::GetInstance()->LoadTextureGraph("select.png"));
	selectCursor_->SetPosition(Vector2D(200, 420));
	selectCursor_->SetAnchorPoint(Vector2D(0.5f, 0.5f));

#pragma endregion
}

void TitleScene::Initialize()
{
	//	Fog
	LightManager::GetInstance()->SetFogActive(true);
	LightManager::GetInstance()->SetFogStart(0.55f);
	LightManager::GetInstance()->SetFogEnd(2.5f);
	LightManager::GetInstance()->SetFogFar(40.0f);

	//	カーソル固定解除
	InputManager::GetInstance()->GetMouse()->SetLockCursor(false);

	optionScene_ = std::make_unique<OptionScene>();
	optionScene_->Initialize("Option");

	LoadResources();

	selectMord_ = GameStart;

	//	Camera
	std::unique_ptr<TitleCamera> camera = std::make_unique<TitleCamera>();
	camera->Initialize(Vector3D(0.5f, 12.0f, -22.0f), Vector3D(1.0f,3.0f,0.5f), Vector3D(0, 1, 0));
	CameraManager::GetInstance()->SetMainCamera(std::move(camera));
	//	LightCamera
	CameraManager::GetInstance()->GetLightCamera()->SetEye(Vector3D(78.0f, 50.0f, -30.0f));

	selectCounter_.Initialize(40, true, true);
	selectCounter_.SetIsEndless(true);
	selectCounter_.SetIsActive(true);

	//XAudioManager::GetInstance()->PlaySoundWave("title.wav", XAudioManager::BGM, true);
}

void TitleScene::Finalize()
{
	XAudioManager::GetInstance()->StopAllSound();
	XAudioManager::GetInstance()->DeleteAllSound();
}

void TitleScene::MatUpdate()
{
	selectCursor_->Update();

	//	モデル
	level_->MatUpdate();
	ground_->MatUpdate();
	skydome_->MatUpdate();

	optionScene_->Update();
}

void TitleScene::Update()
{
	bool select = InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button);

	selectCounter_.Update();

	if (select)
	{
		XAudioManager::GetInstance()->PlaySoundWave("decision.wav", XAudioManager::SE);

		if (uiDrawer_->GetActiveTagName() == "Title") {
			if (uiDrawer_->GetActiveButtonName() == "Test") {
				uiDrawer_->SetUI("Menu");
			}
		}

		else if (uiDrawer_->GetActiveTagName() == "Menu") {
			if (uiDrawer_->GetActiveButtonName() == "Game") {
				SceneManager::GetInstance()->SetNextScene("GAMESCENE");
			}

			if (uiDrawer_->GetActiveButtonName() == "Option") {
				optionScene_->SetIsActive(true);
			}

			if (uiDrawer_->GetActiveButtonName() == "Exit") {
				SceneManager::GetInstance()->GameLoopEnd();
			}
		}
	}

	//else if (ui_->GetTags() & UIData::Tag3) {
	//	optionScene_->Update();
	//}
	float size = Easing::EaseIn(1.0f, 1.05f, selectCounter_.GetCountPerMaxCount(), 2);
	Vector2D cursorSize(298, 82);
	selectCursor_->SetSize(cursorSize * size);

	InputManager* inputMan = InputManager::GetInstance();
	int16_t inputValue = inputMan->GetTriggerKeyAndButton(DIK_W, InputJoypad::DPAD_Up) -
		inputMan->GetTriggerKeyAndButton(DIK_S, InputJoypad::DPAD_Down);
	uiDrawer_->Update(inputValue);

	MatUpdate();
}

void TitleScene::ImguiUpdate()
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("TitleScene");

	imguiMan->Text("mord : %d", selectMord_);

	float value = SceneManager::GetInstance()->GetDissolveValue();
	imguiMan->SetSliderFloat("dissolve", value, 0.01f, 0.0f, 1.0f);
	SceneManager::GetInstance()->SetDissolveValue(value);

	if (imguiMan->SetButton("Start"))	selectMord_ = GameStart;
	if (imguiMan->SetButton("Option"))	selectMord_ = Option;
	if (imguiMan->SetButton("End"))		selectMord_ = GameEnd;
	imguiMan->CheckBox("DrawUI", drawUI_);

	imguiMan->Text("Option : %f", optionScene_->GetIsActive() ? "True" : "False");

	imguiMan->EndWindow();
}

void TitleScene::DrawShadow() 
{
	level_->Draw(true);
	ground_->Draw(true);
	skydome_->Draw(true);
}

void TitleScene::Draw()
{
	//	ステージ描画
	level_->Draw(false);
	ground_->Draw(false);
	skydome_->Draw(false);

	if (!drawUI_) return;
	uiDrawer_->Draw();

	selectCursor_->Draw();

	optionScene_->Draw();
}
