#include "MyDebugCamera.h"
#include "InputManager.h"
#include "MyMath.h"

#include "Quaternion.h"

#include "ImGuiManager.h"

void MyDebugCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	mouse_ = InputManager::GetInstance()->GetMouse();
	keyboard_ = InputManager::GetInstance()->GetKeyboard();

	ICamera::Initialize(eye, target, up);
}

void MyDebugCamera::SetMoveMode(bool active)
{
	if (!active) return;

	bool dikShift = keyboard_->GetKey(DIK_LSHIFT) || keyboard_->GetKey(DIK_RSHIFT);

	if (dikShift)	mode_ = TranslationMove;
	else			mode_ = RotationMove;
}

void MyDebugCamera::CalcDisEyeToTarget()
{
	//	キー入力
	disEyeTarget_ -= mouse_->GetWheel() * (disEyeTarget_ * frontMoveSpd_);
	//	範囲設定
	disEyeTarget_ = MyMath::mMax(disEyeTarget_, MIN_EYE_TO_TARGET);
}

Vector3D MyDebugCamera::CalcTransMove(bool active)
{
	Vector3D ans;

	//	前後移動
	ans += frontVec_ * (float)(keyboard_->GetKey(DIK_X) - keyboard_->GetKey(DIK_Z)) * transSpd_;

	if (!active)				return ans;
	if (mode_ == RotationMove)	return ans;

	Vector2D moveCursor = mouse_->GetMoveCursor();
	moveCursor.Normalize();
	//	左右移動
	ans -= rightVec_ * (float)(moveCursor.x) * transSpd_;
	//	上下移動
	ans -= downVec_ * (float)(moveCursor.y) * transSpd_;

	return ans;
}

void MyDebugCamera::CalcRotMove(bool active)
{
	if (!active)					return;
	if (mode_ == TranslationMove)	return;

	Vector2D moveCursor = mouse_->GetMoveCursor();
	moveCursor /= mouseMoveRate_;

	rotValue_ = moveCursor;
}

void MyDebugCamera::SetPosition(const Vector3D& moveTarget)
{
	Quaternion rightMove = MakeAxisAngle(rightVec_, rotValue_.y);
	Quaternion upMove = MakeAxisAngle(Vector3D(0, 1, 0), rotValue_.x);
	Quaternion qMove = upMove * rightMove;
	rightVec_ = RotateVector(rightVec_, qMove);
	rightVec_.Normalize();
	frontVec_ = RotateVector(frontVec_, qMove);
	frontVec_.Normalize();
	up_ = frontVec_.cross(rightVec_);
	downVec_ = -up_;

	target_ += moveTarget;
	eye_ = target_ - disEyeTarget_ * frontVec_;
}

void MyDebugCamera::Update()
{
	rotValue_ = Vector2D();
	bool dikWheel = mouse_->GetClick(InputMouse::WheelClick);

	SetMoveMode(mouse_->GetClickTrigger(InputMouse::WheelClick));

	CalcDisEyeToTarget();

	Vector3D moveTarget = CalcTransMove(dikWheel);

	CalcRotMove(dikWheel);

	//	座標更新
	SetPosition(moveTarget);

	ICamera::Update();
}

void MyDebugCamera::ImGuiInfo()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->Text("Mode : %d", mode_);
	imgui->Text("     : 0->Trans 1->Rot");

	imgui->Text("RotValue  : (%.2f, %.2f)", rotValue_.x, rotValue_.y);
	float dot;
	dot = rightVec_.dot(downVec_);
	imgui->Text("front : %f", dot);
	dot = frontVec_.dot(downVec_);
	imgui->Text("right : %f", dot);
	dot = rightVec_.dot(frontVec_);
	imgui->Text("down : %f", dot);

	imgui->Text("DisEyeTarget : %f", disEyeTarget_);
}
