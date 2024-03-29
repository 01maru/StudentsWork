#include "InputJoypad.h"
#include "Vector2D.h"
#include "ImGuiManager.h"
#include "MyMath.h"

using namespace MyMath;

void MNE::InputJoypad::SetDeadZone(int16_t& sThumb, int32_t deadzone)
{
	if ((sThumb < deadzone) && sThumb > -deadzone) {
		sThumb = 0;
	}
	else {
		//	マイナスの時
		if (sThumb < 0)
		{
			sThumb = static_cast<int16_t>(MyMath::mMin(sThumb + deadzone, 0));
		}
		//	プラスの時
		else
		{
			sThumb = static_cast<int16_t>(MyMath::mMax(sThumb - deadzone, 0));
		}
		inputted_ = true;
	}
}

void MNE::InputJoypad::Update()
{
    prevState_ = state_;

    //  有効か設定
    active_ = ERROR_SUCCESS == XInputGetState(0, &state_);

	//	無効だったらその後の処理飛ばす
	if (!active_) return;
	
	inputted_ = ((state_.Gamepad.wButtons | 0) != 0 ||
		state_.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ||
		state_.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

    //  DeadZone設定
    SetDeadZone(state_.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    SetDeadZone(state_.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
    SetDeadZone(state_.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
    SetDeadZone(state_.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
}

void MNE::InputJoypad::ImGuiUpdateVibration(ImGuiManager* imgui)
{
	int vibL = (int)vibration_.wLeftMotorSpeed;
	int vibR = (int)vibration_.wRightMotorSpeed;
	imgui->SetSliderInt("Vibration Left", vibL, 10.0f, 0, 65535);
	imgui->SetSliderInt("Vibration Right", vibR, 10.0f, 0, 65535);
	vibration_.wLeftMotorSpeed = (WORD)vibL;
	vibration_.wRightMotorSpeed = (WORD)vibR;

	imgui->CheckBox("ActiveVibration", activeVibration_);
	if (activeVibration_) SetVibration(Vector2D(vibL, vibR));
}

void MNE::InputJoypad::ImGuiUpdateButton(ImGuiManager* imgui)
{
	if (!imgui->TreeNode("Button")) return;

	imgui->Text("Start Button  : %s\nTrigger : %s\nRelease : %s\n"
		, GetButton(START_Button) ? "True" : "False"
		, GetButtonTrigger(START_Button) ? "True" : "False"
		, GetButtonRelease(START_Button) ? "True" : "False");

	imgui->Text("Back Button  : %s\nTrigger : %s\nRelease : %s\n"
		, GetButton(BACK_Button) ? "True" : "False"
		, GetButtonTrigger(BACK_Button) ? "True" : "False"
		, GetButtonRelease(BACK_Button) ? "True" : "False");

	imgui->Text("A Button  : %s\nTrigger : %s\nRelease : %s\n"
		, GetButton(A_Button) ? "True" : "False"
		, GetButtonTrigger(A_Button) ? "True" : "False"
		, GetButtonRelease(A_Button) ? "True" : "False");

	imgui->Text("B Button  : %s\nTrigger : %s\nRelease : %s\n"
		, GetButton(B_Button) ? "True" : "False"
		, GetButtonTrigger(B_Button) ? "True" : "False"
		, GetButtonRelease(B_Button) ? "True" : "False");

	imgui->Text("X Button  : %s\nTrigger : %s\nRelease : %s\n"
		, GetButton(X_Button) ? "True" : "False"
		, GetButtonTrigger(X_Button) ? "True" : "False"
		, GetButtonRelease(X_Button) ? "True" : "False");

	imgui->Text("Y Button  : %s\nTrigger : %s\nRelease : %s\n"
		, GetButton(Y_Button) ? "True" : "False"
		, GetButtonTrigger(Y_Button) ? "True" : "False"
		, GetButtonRelease(Y_Button) ? "True" : "False");

	imgui->Text("Left Button  : %s\nTrigger : %s\nRelease : %s\n"
		, GetButton(Left_Button) ? "True" : "False"
		, GetButtonTrigger(Left_Button) ? "True" : "False"
		, GetButtonRelease(Left_Button) ? "True" : "False");

	imgui->Text("Right Button  : %s\nTrigger : %s\nRelease : %s\n"
		, GetButton(Right_Button) ? "True" : "False"
		, GetButtonTrigger(Right_Button) ? "True" : "False"
		, GetButtonRelease(Right_Button) ? "True" : "False");

	imgui->TreePop();
}

void MNE::InputJoypad::ImGuiUpdateDPAD(ImGuiManager* imgui)
{
	if (!imgui->TreeNode("DPAD")) return;

	imgui->Text("DPAD UP : %s\nTrigger : %s\nRelease : %s\n"
		, GetButton(DPAD_Up) ? "True" : "False"
		, GetButtonTrigger(DPAD_Up) ? "True" : "False"
		, GetButtonRelease(DPAD_Up) ? "True" : "False");

	imgui->Text("DPAD DOWN  : %s\nTrigger : %s\nRelease : %s\n"
		, GetButton(DPAD_Down) ? "True" : "False"
		, GetButtonTrigger(DPAD_Down) ? "True" : "False"
		, GetButtonRelease(DPAD_Down) ? "True" : "False");

	imgui->Text("DPAD Left : %s\nTrigger : %s\nRelease : %s\n"
		, GetButton(DPAD_Left) ? "True" : "False"
		, GetButtonTrigger(DPAD_Left) ? "True" : "False"
		, GetButtonRelease(DPAD_Left) ? "True" : "False");

	imgui->Text("DPAD Right : %s\nTrigger : %s\nRelease : %s\n"
		, GetButton(DPAD_Right) ? "True" : "False"
		, GetButtonTrigger(DPAD_Right) ? "True" : "False"
		, GetButtonRelease(DPAD_Right) ? "True" : "False");

	imgui->TreePop();
}

void MNE::InputJoypad::ImGuiUpdateStick(ImGuiManager* imgui)
{
	if (!imgui->TreeNode("Stick")) return;

	Vector2D pos = GetThumbL();
	imgui->Text("LStick Pos  : (%.f, %.f)", pos.x, pos.y);
	imgui->Text("LStick TriggerX  : %s LStick TriggerY  : %s"
		, GetTriggerThumbLX() ? "True" : "False"
		, GetTriggerThumbLY() ? "True" : "False");

	imgui->Text("LStick Button  : %s\nTrigger : %s\nRelease : %s\n"
		, GetButton(LStick_Button) ? "True" : "False"
		, GetButtonTrigger(LStick_Button) ? "True" : "False"
		, GetButtonRelease(LStick_Button) ? "True" : "False");

	pos = GetThumbR();
	imgui->Text("RStick Pos  : (%.f, %.f)", pos.x, pos.y);
	imgui->Text("RStick TriggerX  : %s RStick TriggerY  : %s"
		, GetTriggerThumbRX() ? "True" : "False"
		, GetTriggerThumbRY() ? "True" : "False");

	imgui->Text("RStick Button  : %s\nTrigger : %s\nRelease : %s\n"
		, GetButton(RStick_Button) ? "True" : "False"
		, GetButtonTrigger(RStick_Button) ? "True" : "False"
		, GetButtonRelease(RStick_Button) ? "True" : "False");

	imgui->TreePop();
}

void MNE::InputJoypad::ImGuiUpdateTrigger(ImGuiManager* imgui)
{
	if (!imgui->TreeNode("Trigger")) return;

	imgui->Text("L Trigger : %s\nValue : %d\n"
		, GetLTrigger() ? "True" : "False"
		, GetLTriggerValue());

	imgui->Text("R Trigger : %s\nValue : %d\n"
		, GetRTrigger() ? "True" : "False"
		, GetRTriggerValue());

	imgui->TreePop();
}

void MNE::InputJoypad::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	if (!imgui->TreeNode("JoyPad")) return;

	imgui->Text("Inputted  : %s", inputted_ ? "True" : "False");

	imgui->Text("Active  : %s", active_ ? "True" : "False");

	ImGuiUpdateVibration(imgui);

	ImGuiUpdateButton(imgui);

	ImGuiUpdateDPAD(imgui);

	ImGuiUpdateStick(imgui);

	ImGuiUpdateTrigger(imgui);
	
	imgui->TreePop();
}

bool MNE::InputJoypad::GetButton(JoyPadButton button)
{
	if (!active_) return false;

    return (state_.Gamepad.wButtons & button);
}

bool MNE::InputJoypad::GetButtonTrigger(JoyPadButton button)
{
	if (!active_) return false;

    return (state_.Gamepad.wButtons & button) && !(prevState_.Gamepad.wButtons & button);
}

bool MNE::InputJoypad::GetButtonRelease(JoyPadButton button)
{
	if (!active_) return false;

	return !(state_.Gamepad.wButtons & button) && (prevState_.Gamepad.wButtons & button);
}

bool MNE::InputJoypad::GetLTrigger()
{
	if (!active_) return false;

    return state_.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

bool MNE::InputJoypad::GetRTrigger()
{
	if (!active_) return false;

    return state_.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

size_t MNE::InputJoypad::GetLTriggerValue()
{
	if (!active_) return 0;

	return state_.Gamepad.bLeftTrigger;
}

size_t MNE::InputJoypad::GetRTriggerValue()
{
	if (!active_) return 0;

	return state_.Gamepad.bRightTrigger;
}

Vector2D MNE::InputJoypad::GetThumbR()
{
	if (!active_) return Vector2D();

    return Vector2D(state_.Gamepad.sThumbRX, state_.Gamepad.sThumbRY);
}

bool MNE::InputJoypad::GetTriggerThumbRX()
{
	if (!active_) return false;

	return  state_.Gamepad.sThumbRX != 0 && prevState_.Gamepad.sThumbRX == 0;
}

bool MNE::InputJoypad::GetTriggerThumbRY()
{
	if (!active_) return false;

	return state_.Gamepad.sThumbRY != 0 && prevState_.Gamepad.sThumbRY == 0;
}

MyMath::Vector2D MNE::InputJoypad::GetThumbL()
{
    return MyMath::Vector2D(state_.Gamepad.sThumbLX, state_.Gamepad.sThumbLY);
}

bool MNE::InputJoypad::GetTriggerThumbLX()
{
	if (!active_) return false;

	return  state_.Gamepad.sThumbLX != 0 && prevState_.Gamepad.sThumbLX == 0;
}

bool MNE::InputJoypad::GetTriggerThumbLY()
{
	if (!active_) return false;

	return state_.Gamepad.sThumbLY != 0 && prevState_.Gamepad.sThumbLY == 0;
}

int32_t MNE::InputJoypad::GetThumbRDeadZone()
{
	return XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
}

void MNE::InputJoypad::SetVibration(const Vector2D& motorSpd)
{
	vibration_.wLeftMotorSpeed = (WORD)motorSpd.x;
	vibration_.wRightMotorSpeed = (WORD)motorSpd.y;

	DWORD vib = XInputSetState(0, &vibration_);
	vib = ERROR_SUCCESS;
}
