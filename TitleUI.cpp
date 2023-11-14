#include "TitleUI.h"
#include "InputManager.h"
#include "XAudioManager.h"
#include "SceneManager.h"
#include "ImGuiManager.h"

void TitleUI::Initialize()
{
	LoadResources();

	cursor_.Initialize();
}

void TitleUI::LoadResources()
{
#pragma region Sound

	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("decision.wav");

#pragma endregion

	//	配置データ
	data_.LoadData("Title");
	//	カーソル
	cursor_.LoadResources();
	//	オプション
	option_.Initialize("Option");
}

void TitleUI::TitleInputUpdate(bool selectButton)
{
	//	オプションシーンだったら終了
	if (option_.GetIsActive() == true) return;

	//	演出が終わっていなかったら処理しない
	if (data_.GetIsEndAnimation() == false) return;

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

	cursor_.SetIsActive(data_.GetIsEndAnimation());
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
	cursor_.SetMinSize(option_.GetSelectScale());
	cursor_.SetMaxSize(option_.GetSelectScale() + Vector2D(16, 8));
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

	//	選択中のモード
	imgui->Text("mord : %s", data_.GetSelectName().c_str());

	option_.ImGuiUpdate();
}

void TitleUI::Draw()
{
	data_.Draw();

	option_.Draw();

	cursor_.Draw();
}
