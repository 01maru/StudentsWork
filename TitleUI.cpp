#include "TitleUI.h"

#include "InputManager.h"
#include "XAudioManager.h"
#include "SceneManager.h"
#include "ImGuiManager.h"
#include "TextureManager.h"


void TitleUI::Initialize()
{
	LoadResources();
	option_.Initialize("Option");

	cursor_.Initialize();
}

void TitleUI::Finalize()
{
}

void TitleUI::LoadResources()
{
	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("decision.wav");
	cursor_.LoadResources();

	TextureManager::GetInstance()->AsyncLoadTextureGraph("select.png");
	data_.LoadData("TitleScene");
}

void TitleUI::TitleInputUpdate(bool selectButton)
{
	//	オプションシーンだったら
	if (option_.GetIsActive() == true) return;

	//	ボタンを選択したら
	if (selectButton)
	{
		//	決定音再生
		XAudioManager::GetInstance()->PlaySoundWave("decision.wav", XAudioManager::SE);

		std::string buttonName = data_.GetSelectName();
		if (buttonName == "Start") {
			SceneManager::GetInstance()->SetNextScene("GAMESCENE");
		}

		else if (buttonName == "Option") {
			option_.SetIsActive(true);
			option_.ResetSelectButton();
		}

		else if (buttonName == "Quit") {
			SceneManager::GetInstance()->GameLoopEnd();
		}
	}

	data_.InputUpdate();

	//	カーソル移動
	cursor_.SetCursorPosition(data_.GetSelectPosition());
}

void TitleUI::TitleUpdate(bool selectButton)
{
	TitleInputUpdate(selectButton);

	data_.Update();
}

void TitleUI::OptionUpdate(bool selectButton)
{
	option_.InputUpdate(selectButton);
	option_.Update();

	//	オプション中だったら
	if (option_.GetIsActive() == false) return;
	//	カーソル移動
	cursor_.SetCursorPosition(option_.GetSelectPosition());
}

void TitleUI::Start()
{
	data_.Initialize();
}

void TitleUI::Update()
{
	bool isSelect = InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button);

	TitleUpdate(isSelect);

	OptionUpdate(isSelect);

	cursor_.Update();
}

void TitleUI::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();
	std::string name = data_.GetSelectName();
	imgui->Text("mord : %s", name.c_str());
	option_.ImGuiUpdate();
}

void TitleUI::Draw()
{
	data_.Draw();

	option_.Draw();

	cursor_.Draw();
}
