#include "PauseScreen.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "XAudioManager.h"
#include "ImGuiManager.h"
#include "CameraManager.h"

void PauseScreen::Initialize()
{
	isActive_ = false;
	
	LoadResources();

	//	配置データ
	data_.Initialize();

	//	カーソル
	cursor_.Initialize();
	cursor_.SetCursorPosition(data_.GetSelectPosition(), false);

	option_.Initialize();
	option_.SetSelectCursor(&cursor_);
}

void PauseScreen::LoadResources()
{
#pragma region Sound

	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("decision.wav");

#pragma endregion

	data_.LoadData("Pause");
	option_.LoadResources("Option");
	cursor_.LoadResources();
}

void PauseScreen::IsActiveUpdate()
{
	if (option_.GetIsActive() == true) return;

	InputManager* input = InputManager::GetInstance();
	bool isSelect = input->GetTriggerKeyAndButton(DIK_ESCAPE, InputJoypad::START_Button);

	//	キー入力されたら
	if (isSelect) {
		isActive_ = !isActive_;

		gameCamera_->SetIsActive(!isActive_);
		MouseCursorInit();
	}
}

void PauseScreen::PauseInputUpdate(bool selectButton)
{
	//	オプションだったら
	if (option_.GetIsActive() == true) return;

	if (selectButton) {
		//	決定音再生
		XAudioManager::GetInstance()->PlaySoundWave("decision.wav", XAudioManager::SE);

		std::string buttonName = data_.GetSelectName();

		//	ゲームに戻る
		if (buttonName == "Resume") {
			isActive_ = false;

			MouseCursorInit();
		}
		//	オプション画面を開く
		else if (buttonName == "Option") {
			option_.SetIsActive(true);
			option_.ResetSelectButton();
			option_.SetCursorBackPos(data_.GetSelectPosition());
			cursor_.SetCursorPosition(option_.GetSelectPosition(), false);
			return;
			return;
		}
		//	タイトルに戻る
		else if (buttonName == "Quit") {
			SceneManager::GetInstance()->SetNextScene("TITLESCENE");
		}
	}

	data_.InputUpdate();

	//	カーソル移動
	cursor_.SetCursorPosition(data_.GetSelectPosition());
}

void PauseScreen::PauseUpdate(bool selectButton)
{
	PauseInputUpdate(selectButton);

	cursor_.SetIsActive(data_.GetIsEndAnimation());
	data_.Update();
}

void PauseScreen::OptionUpdate(bool selectButton)
{
	option_.InputUpdate(selectButton);
	option_.Update();
}

void PauseScreen::MouseCursorInit()
{
	//	ポーズ画面になったら
	bool lockCursor = isActive_ == false;

	//	カーソル表示
	InputManager::GetInstance()->GetMouse()->SetLockCursor(lockCursor);
}

void PauseScreen::Update()
{
	IsActiveUpdate();

	//	ポーズ中じゃなかったら終了
    if (!isActive_) return;

	bool isSelect = InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button);

	PauseUpdate(isSelect);
	OptionUpdate(isSelect);

	cursor_.Update();
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

	cursor_.Draw();
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

void PauseScreen::SetGameCamera(GameCamera* gamecamera)
{
	gameCamera_ = gamecamera;
}
