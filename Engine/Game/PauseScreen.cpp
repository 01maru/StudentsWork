#include "PauseScreen.h"
#include "InputManager.h"
#include "SceneManager.h"

void PauseScreen::Initialize()
{
	isActive_ = false;

	data_.LoadData("Pause");
	data_.Initialize();

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

void PauseScreen::PauseUpdate()
{
	//	オプションだったら
	if (option_.GetIsActive() == true) return;

	data_.InputUpdate();

	if (data_.GetSelectName() == "Continue") {
		isActive_ = false;
	}
	else if (data_.GetSelectName() == "Option") {
		option_.SetIsActive(true);
	}
	else if (data_.GetSelectName() == "Quit") {
		SceneManager::GetInstance()->SetNextScene("TITLESCENE");
	}
}

void PauseScreen::Update()
{
	IsActiveUpdate();

	//	ポーズ中じゃなかったら終了
    if (!isActive_) return;

	PauseUpdate();

	option_.Update();
	
	data_.Update();
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
