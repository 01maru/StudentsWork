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

	//Vector3D vec = middle;
	//vec -= target;
	//float disY = vec.y;
	//float dis = vec.length();
	//float len = dis / 3.0f * 2;
	//target = middle;
	//target -= vec.normalize() * len;

	//Vector2D vec2(vec.x, vec.z);
	//vec2.normalize();
	//float angleY = MyMath::ConvertToRad(20.0f) * dis / 50.0f;
	//angleY -= MyMath::ConvertToRad(5.0f) * disY / 10.0f;

	//vec = Vector3D(vec2.x, tanf(angleY), vec2.y);
	////if (cos(angleY) == 0) vec.y = 0;
	//eye = middle;
	//len = 10.0f;
	//eye += vec * len;
	//if (vec2.length() != 0) {
	//	eye.x -= vec2.x * disY;
	//	eye.z -= vec2.y * disY;
	//}
	//else {
	//	Vector2D modelVec(modelFrontVec->x, modelFrontVec->y);
	//	modelVec.normalize();
	//	eye.x += modelVec.x * disY;
	//	eye.z += modelVec.y * disY;
	//}

	//if (dis > 50.0f) {
	//	lockOn = !lockOn;
	//}
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
	Quaternion qMove = rightMove * upMove;
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
