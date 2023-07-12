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
#pragma endregion
	
	ui_ = std::make_unique<UIData>();
	ui_->LoadSprites("TitleScene");
	ui_->SetTag(UIData::Tag1);
}

void TitleScene::Initialize()
{
	InputManager::GetInstance()->GetMouse()->SetLockCursor(false);

	optionScene_ = std::make_unique<OptionScene>();
	optionScene_->Initialize("Option");

	LoadResources();

#pragma region Sprite
	backSprite_ = std::make_unique<Sprite>();
	backSprite_->Initialize(TextureManager::GetInstance()->GetWhiteTexture());
	backSprite_->SetSize(Vector2D{ Window::sWIN_WIDTH,Window::sWIN_HEIGHT });
	backSprite_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
#pragma endregion

	selectMord_ = GameStart;
}

void TitleScene::Finalize()
{
	XAudioManager::GetInstance()->StopAllSound();
	XAudioManager::GetInstance()->DeleteAllSound();
}

void TitleScene::MatUpdate()
{
	//	背景
	backSprite_->Update();
	ui_->Update();

	optionScene_->Update();
}

void TitleScene::Update()
{
	if (ui_->GetTags() & UIData::Tag1) {
		if (InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button)) {
			ui_->SetTag(UIData::Tag2);
		}
	}
	else if (ui_->GetTags() & UIData::Tag2) {
		if (InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button)) {
			switch (selectMord_)
			{
			case GameStart:
				SceneManager::GetInstance()->SetNextScene("GAMESCENE");
				break;
			case Option:
				optionScene_->SetIsActive(true);
				ui_->SetTag(UIData::Tag3);
				break;
			case GameEnd:
				SceneManager::GetInstance()->GameLoopEnd();
				break;
			}
		}
	}
	else if (ui_->GetTags() & UIData::Tag3) {
		optionScene_->Update();
	}

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
	//	背景
	backSprite_->Draw();

	if (!drawUI_) return;
	ui_->Draw();

	optionScene_->Draw();
}
