﻿#include "MyDebugCamera.h"
#include "InputManager.h"
#include "MyMath.h"
#include <cmath>

#include "Quaternion.h"

#include "ImGuiManager.h"

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
	disEyeTarget_ -= mouse_->GetWheel() * (disEyeTarget_ * 0.001f);
	//	範囲設定
	disEyeTarget_ = MyMath::mMax(disEyeTarget_, MIN_EYE_TO_TARGET);
}

Vector3D MyDebugCamera::CalcTransMove(bool active)
{
	Vector3D ans;

	//	前後移動
	ans += frontVec_ * (float)(keyboard_->GetKey(DIK_X) - keyboard_->GetKey(DIK_Z)) * 0.1f;

	if (!active)				return ans;
	if (mode_ == RotationMove)	return ans;

	Vector2D moveCursor = mouse_->GetMoveCursor();
	moveCursor.Normalize();
	//	左右移動
	ans -= rightVec_ * (float)(moveCursor.x);
	//	上下移動
	ans -= downVec_ * (float)(moveCursor.y);

	return ans;
}

void MyDebugCamera::CalcRotMove(bool active)
{
	if (!active)					return;
	if (mode_ == TranslationMove)	return;

	Vector2D moveCursor = mouse_->GetMoveCursor();
	moveCursor /= 1000;

	if (up_.y < 0) moveCursor.x = -moveCursor.x;

	rotValue_ = moveCursor;
}

void MyDebugCamera::SetPosition(const Vector3D& moveTarget)
{
	Quaternion upMove = MakeAxisAngle(up_, rotValue_.x);
	Quaternion rightMove = MakeAxisAngle(rightVec_, rotValue_.y);
	Quaternion qMove = upMove * rightMove;
	frontVec_ = RotateVector(frontVec_, qMove);

	target_ += moveTarget;
	up_ = RotateVector(up_, qMove);
	eye_ = target_ - disEyeTarget_ * frontVec_;
}

void MyDebugCamera::Initialize(const Vector3D& /*frontVec*/, const Vector3D& /*center*/, float /*dis*/)
{
}

void MyDebugCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	mouse_ = InputManager::GetInstance()->GetMouse();
	keyboard_ = InputManager::GetInstance()->GetKeyboard();

	//SetProjectionMatrix(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, MyMath::ConvertToRad(90.0f));

	eye_ = eye;
	target_ = target;
	up_ = up;

	MatUpdate();

	//	disEyeTraget初期化
	disEyeTarget_ = Vector3D(target_ - eye_).GetLength();

	//	方向ベクトル
	CalcDirectionVec();
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

	//	方向ベクトル
	CalcDirectionVec();

	//	ビルボード
	CalcBillboard();

	MatUpdate();
}

void MyDebugCamera::ImGuiInfo()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->Text("Mode : %d", mode_);
	imgui->Text("     : 0->Trans 1->Rot");

	imgui->Text("RotValue  : (%.2f, %.2f)", rotValue_.x, rotValue_.y);

	imgui->Text("DisEyeTarget : %f", disEyeTarget_);
}
