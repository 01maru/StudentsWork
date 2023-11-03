#include "PauseScreen.h"
#include "InputManager.h"
#include "SceneManager.h"

#include "ImGuiManager.h"

void PauseScreen::Initialize()
{
	isActive_ = false;
	
	LoadResources();

	data_.Initialize();
}

void PauseScreen::LoadResources()
{
	data_.LoadData("Pause");
	option_.Initialize("Option");
}

void PauseScreen::IsActiveUpdate()
{
	InputManager* input = InputManager::GetInstance();

	if (input->GetTriggerKeyAndButton(DIK_ESCAPE, InputJoypad::START_Button)) {
		isActive_ = !isActive_;

		//	ポーズ画面になったら
		if (isActive_ == true) {
			//	カーソル表示
			InputManager::GetInstance()->GetMouse()->SetLockCursor(false);
		}
	}
}

void PauseScreen::PauseInputUpdate(bool selectButton)
{
	//	オプションだったら
	if (option_.GetIsActive() == true) return;

	if (selectButton) {
		std::string buttonName = data_.GetSelectName();

		if (buttonName == "Resume") {
			isActive_ = false;
		}

		else if (buttonName == "Option") {
			option_.SetIsActive(true);
		}

		else if (buttonName == "Quit") {
			SceneManager::GetInstance()->SetNextScene("TITLESCENE");
		}
	}

	data_.InputUpdate();

	//	カーソル移動

}

void PauseScreen::PauseUpdate(bool selectButton)
{
	PauseInputUpdate(selectButton);

	data_.Update();
}

void PauseScreen::OptionUpdate(bool selectButton)
{
	option_.InputUpdate(selectButton);
	option_.Update();

	//	オプション中だったら
	if (option_.GetIsActive() == false) return;
	//	カーソル移動
}

void PauseScreen::Update()
{
	IsActiveUpdate();

	//	ポーズ中じゃなかったら終了
    if (!isActive_) return;

	bool isSelect = InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button);

	PauseUpdate(isSelect);
	OptionUpdate(isSelect);
}

void PauseScreen::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->Text("Pause : %s", isActive_ ? "True" : "False");
	imgui->Text("SelectButton : %s", data_.GetSelectName().c_str());

	option_.ImGuiUpdate();
}

void PauseScreen::Draw()
{
    if (!isActive_) return;

    data_.Draw();

	option_.Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool PauseScreen::GetIsActive()
{
	return isActive_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void PauseScreen::SetIsActive(bool isActive)
{
    isActive_ = isActive;
}
