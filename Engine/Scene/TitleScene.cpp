#include "TitleScene.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "XAudioManager.h"
#include "Window.h"

#include "ImGuiManager.h"

void TitleScene::LoadResources()
{
#pragma region Sound
	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("cursorMove.wav");
	xAudioMan->LoadSoundWave("decision.wav");
	//	タイトル音
#pragma endregion

#pragma region Model
	//	ステージ読み込み
#pragma endregion
	
#pragma region UI

	uiDrawer_ = std::make_unique<UIDrawer>();
	uiDrawer_->LoadSprites("TitleScene");
	uiDrawer_->SetUI("Title");

#pragma endregion
}

void TitleScene::Initialize()
{
	//	カーソル固定解除
	InputManager::GetInstance()->GetMouse()->SetLockCursor(false);

	optionScene_ = std::make_unique<OptionScene>();
	optionScene_->Initialize("Option");

	LoadResources();

	selectMord_ = GameStart;
}

void TitleScene::Finalize()
{
	XAudioManager::GetInstance()->StopAllSound();
	XAudioManager::GetInstance()->DeleteAllSound();
}

void TitleScene::MatUpdate()
{
	//	モデル

	optionScene_->Update();
}

void TitleScene::Update()
{
	bool select = InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button);

	if (select)
	{

		if (uiDrawer_->GetActiveTagName() == "Title") {
			uiDrawer_->SetUI("Menu");
		}

		else if (uiDrawer_->GetActiveTagName() == "Menu") {
			uiDrawer_->SetUI("Menu");
		}
		//{
		//case GameStart:
		//	SceneManager::GetInstance()->SetNextScene("GAMESCENE");
		//	break;
		//case Option:
		//	optionScene_->SetIsActive(true);
		//	ui_->SetTag(UIData::Tag3);
		//	break;
		//case GameEnd:
		//	SceneManager::GetInstance()->GameLoopEnd();
		//	break;
		//}
	}

	//else if (ui_->GetTags() & UIData::Tag3) {
	//	optionScene_->Update();
	//}

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

	if (imguiMan->SetButton("Start"))	selectMord_ = GameStart;
	if (imguiMan->SetButton("Option"))	selectMord_ = Option;
	if (imguiMan->SetButton("End"))		selectMord_ = GameEnd;
	imguiMan->CheckBox("DrawUI", drawUI_);

	imguiMan->Text("Option : %f", optionScene_->GetIsActive() ? "True" : "False");

	imguiMan->EndWindow();
}

void TitleScene::DrawShadow() {}

void TitleScene::Draw()
{
	//	ステージ描画

	if (!drawUI_) return;
	uiDrawer_->Draw();

	optionScene_->Draw();
}
