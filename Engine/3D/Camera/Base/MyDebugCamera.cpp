#include "MyDebugCamera.h"
#include "InputManager.h"
#include "MyMath.h"
#include <cmath>

#include "Quaternion.h"

void MyDebugCamera::SetMoveMode(bool active)
{
	InputKeyboard* keyboard = InputManager::GetInstance()->GetKeyboard();

	if (!active) return;

	bool dikShift = keyboard->GetKey(DIK_LSHIFT) || keyboard->GetKey(DIK_RSHIFT);

	if (dikShift)	mode_ = TranslationMove;
	else			mode_ = RotationMove;
}

void MyDebugCamera::CalcDisEyeToTarget()
{
	InputMouse* mouse = InputManager::GetInstance()->GetMouse();

	//	キー入力
	disEyeTarget_ -= mouse->GetWheel() * (disEyeTarget_ * 0.001f);
	//	範囲設定
	float minDis_ = 10.0f;	//	最小値
	disEyeTarget_ = MyMath::mMax(disEyeTarget_, minDis_);
}

Vector3D MyDebugCamera::CalcMoveTarget(bool active, Vector2D& moveCursor)
{
	InputKeyboard* keyboard = InputManager::GetInstance()->GetKeyboard();
	Vector3D ans;

	//	前後移動
	ans += -frontVec_ * (float)(keyboard->GetKey(DIK_Z) - keyboard->GetKey(DIK_X)) * 0.1f;

	if (!active)				return ans;
	if (mode_ == RotationMove)	return ans;

	moveCursor.Normalize();
	//	左右移動
	ans -= rightVec_ * (float)(moveCursor.x) * 0.1f;
	//	上下移動
	ans -= downVec_ * (float)(moveCursor.y) * 0.1f;

	return ans;
}

void MyDebugCamera::CalcRotValue(bool active, Vector2D& moveCursor)
{
	if (!active)					return;
	if (mode_ == TranslationMove)	return;

	moveCursor /= 1000;

	if (up_.y < 0) moveCursor.x = -moveCursor.x;

	rotValue_ = moveCursor;
}

void MyDebugCamera::SetPosition(const Vector3D& moveTarget)
{
	target_ += moveTarget;

	Quaternion pRightAngle(rotValue_.x, up_);
	Quaternion pUpAngle(rotValue_.y, rightVec_);

	Quaternion q = pRightAngle * pUpAngle;
	Quaternion qq = Conjugate(q);

	Quaternion qPos(0.0f, eye_);
	qPos = q * qPos * qq;
	eye_ = qPos.GetVector3();

	Quaternion qCameraUp(0.0f, up_);
	qCameraUp = q * qCameraUp * qq;

	up_ = MyMath::GetAxis(qCameraUp);
	up_.y = -up_.y;
}

void MyDebugCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	SetProjectionMatrix(Window::sWIN_WIDTH, Window::sWIN_HEIGHT, MyMath::ConvertToRad(90.0f));

	eye_ = eye;
	target_ = target;
	up_ = up;

	MatUpdate();

	//	disEyeTraget初期化
	frontVec_ = target - eye;
	disEyeTarget_ = frontVec_.GetLength();

	//	方向ベクトル
	CalcDirectionVec();
}

void MyDebugCamera::Update()
{
	InputKeyboard* keyboard = InputManager::GetInstance()->GetKeyboard();
	InputMouse* mouse = InputManager::GetInstance()->GetMouse();

	Vector2D moveCursor = mouse->GetCursor() - mouse->GetPrevCursor();

	bool active = mouse->GetClickTrigger(InputMouse::WheelClick);

	SetMoveMode(active);

	CalcDisEyeToTarget();

	Vector3D moveTarget = CalcMoveTarget(active, moveCursor);

	CalcRotValue(active, moveCursor);

	target_ += moveTarget;

	//	範囲　0　>　cursorPos　>　PIx2　に設定
	if (rotValue_.x >= MyMath::PIx2) rotValue_.x -= MyMath::PIx2;
	if (rotValue_.x < 0) rotValue_.x += MyMath::PIx2;
	if (rotValue_.y >= MyMath::PIx2) rotValue_.y -= MyMath::PIx2;
	if (rotValue_.y < 0) rotValue_.y += MyMath::PIx2;

	//	上方向ベクトルと視点座標更新
	up_.y = cosf(rotValue_.y);
	eye_.x = target_.x - disEyeTarget_ * cosf(rotValue_.y) * sinf(rotValue_.x);
	eye_.y = target_.y + disEyeTarget_ * sinf(rotValue_.y);
	eye_.z = target_.z - disEyeTarget_ * cosf(rotValue_.y) * cosf(rotValue_.x);

	//	方向ベクトル
	CalcDirectionVec();

	//	ビルボード
	CalcBillboard();

	MatUpdate();
	//InputMouse* mouse = InputManager::GetInstance()->GetMouse();

	//Vector2D moveCursor = mouse->GetCursor() - mouse->GetPrevCursor();

	//bool active = mouse->GetClickTrigger(InputMouse::WheelClick);

	//SetMoveMode(active);

	//CalcDisEyeToTarget();

	//Vector3D moveTarget = CalcMoveTarget(active, moveCursor);

	//CalcRotValue(active, moveCursor);
	//
	////	範囲　0　>　cursorPos　>　PIx2　に設定
	////if (rotValue_.x >= MyMath::PIx2) rotValue_.x -= MyMath::PIx2;
	////if (rotValue_.x < 0) rotValue_.x += MyMath::PIx2;
	////if (rotValue_.y >= MyMath::PIx2) rotValue_.y -= MyMath::PIx2;
	////if (rotValue_.y < 0) rotValue_.y += MyMath::PIx2;

	//SetPosition(moveTarget);

	//CalcDirectionVec();

	//CalcBillboard();

	//MatUpdate();
}
