#include "GameCamera.h"
#include "ImGuiManager.h"
#include "InputManager.h"
#include "Quaternion.h"

void GameCamera::Initialize(const Vector3D& frontVec, const Vector3D& center, float dis)
{
	InputManager::GetInstance()->GetMouse()->SetLockCursor(true);
	lockOn_ = false;

	ICamera::Initialize(frontVec, center, dis);
}

void GameCamera::LockOnUpdate()
{
	if (!lockOn_) return;

}

void GameCamera::UnLockOnUpdate()
{
	if (lockOn_) return;

	InputManager* input = InputManager::GetInstance();
	InputJoypad* pad = input->GetPad();
	InputMouse* mouse = input->GetMouse();

	//	PadInfo
	Vector2D padVec = pad->GetThumbR();
	padVec.Normalize();

	//	MouseInfo
	Vector2D mouseVec = mouse->GetMoveCursor();
	mouseVec.Normalize();

	//	Mouse&Pad
	Vector2D moveVec = padVec + mouseVec;
	moveVec.Normalize();
	moveVec *= input->GetSensitivity() * 0.1f;

	//	座標更新
	Quaternion upMove = MakeAxisAngle(up_, moveVec.x);
	rightVec_ = RotateVector(rightVec_, upMove);
	Quaternion rightMove = MakeAxisAngle(rightVec_, moveVec.y);
	Quaternion qMove = rightMove * upMove;
	frontVec_ = RotateVector(frontVec_, qMove);
	eye_ = target_ - disEyeTarget_ * frontVec_;
	CalcDirectionVec();
}

void GameCamera::Update()
{
	//InputManager* input = InputManager::GetInstance();

	//	LockOn
	//if (input->GetTriggerKeyAndButton(DIK_Q, InputJoypad::RStick_Button)) lockOn_ = !lockOn_;

	LockOnUpdate();

	UnLockOnUpdate();

	ICamera::Update();
}

void GameCamera::ImGuiInfo()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->Text("LockOn : %s", lockOn_ ? "TRUE" : "FALSE");
}
