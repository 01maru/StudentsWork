#include "GameCamera.h"
#include "ImGuiManager.h"
#include "InputManager.h"
#include "Quaternion.h"

void GameCamera::Initialize(const Vector3D& frontVec, const Vector3D& center, float dis)
{
	InputManager::GetInstance()->GetMouse()->SetLockCursor(true);
	lockOn_ = false;

	target_ = center;
	disEyeTarget_ = dis;

	Vector3D front = frontVec;
	front.Normalize();
	
	eye_ = target_ - disEyeTarget_ * front;

	MatUpdate();

	CalcDirectionVec();
}

void GameCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	eye_ = eye;
	target_ = target;
	up_ = up;

	MatUpdate();

	CalcDirectionVec();
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
	moveVec *= 0.01f;	//	感度変える

	//	座標更新
	Quaternion upMove = MakeAxisAngle(up_, moveVec.x);
	Quaternion rightMove = MakeAxisAngle(rightVec_, moveVec.y);
	Quaternion qMove = upMove * rightMove;
	frontVec_ = RotateVector(frontVec_, qMove);

	up_ = RotateVector(up_, qMove);
	eye_ = target_ - disEyeTarget_ * frontVec_;

	//if (eye.y <= 0) {
	//	eye.y = 0;
	//	Vector2D lenVec(eye.x, eye.z);
	//	lenVec -= Vector2D(target.x, target.z);
	//	float maxlen = 5.0f;
	//	if (lenVec.length() < maxlen) {
	//		lenVec.normalize();
	//		eye.x = target.x + lenVec.x * maxlen;
	//		eye.z = target.z + lenVec.y * maxlen;

	//		float sinY = -target.y / maxlen;
	//		float cosY = -lenVec.x * maxlen / disEyeTarget / sinf(angle.x);
	//		double acosY, asinY;
	//		acosY = acos(cosY);
	//		asinY = asin(sinY);
	//		if (asinY > 0.0) {
	//			cursorSpd.y = acosY;
	//		}
	//		else {
	//			cursorSpd.y = MyMath::PIx2 - acosY;
	//		}
	//	}
	//}
}

void GameCamera::Update()
{
	InputManager* input = InputManager::GetInstance();

	//	LockOn
	if (input->GetTriggerKeyAndButton(DIK_Q, InputJoypad::RStick_Button)) lockOn_ = !lockOn_;

	LockOnUpdate();

	UnLockOnUpdate();

	CalcDirectionVec();

	CalcBillboard();

	MatUpdate();
}

void GameCamera::ImGuiInfo()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->Text("LockOn : %s", lockOn_ ? "TRUE" : "FALSE");
}
