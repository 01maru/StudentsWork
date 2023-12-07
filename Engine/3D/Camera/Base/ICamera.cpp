#include "ICamera.h"
#include "ImGuiManager.h"
#include "Window.h"

using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void ICamera::Initialize(const Vector3D& frontVec, const Vector3D& center, float dis)
{
	matProj_ = MyMath::PerspectiveFovLH(Window::sWIN_WIDTH, Window::sWIN_HEIGHT,
		MyMath::ConvertToRad(50.0f), 0.1f, 1000.0f);

	target_ = center;
	disEyeTarget_ = dis;

	Vector3D front = frontVec;
	front.Normalize();

	eye_ = target_ - disEyeTarget_ * front;

	MatUpdate();

	CalcDirectionVec();
}

void ICamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	matProj_ = MyMath::PerspectiveFovLH(Window::sWIN_WIDTH, Window::sWIN_HEIGHT,
		MyMath::ConvertToRad(50.0f), 0.1f, 1000.0f);

	eye_ = eye;
	target_ = target;
	up_ = up;

	MatUpdate();

	CalcDirectionVec();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void ICamera::CalcBillboard()
{
	//	行列初期化
	billboard_.Identity();

#pragma region 全方向ビルボード
	billboard_.m[0][0] = rightVec_.x;
	billboard_.m[0][1] = rightVec_.y;
	billboard_.m[0][2] = rightVec_.z;
	billboard_.m[1][0] = -downVec_.x;
	billboard_.m[1][1] = -downVec_.y;
	billboard_.m[1][2] = -downVec_.z;
	billboard_.m[2][0] = frontVec_.x;
	billboard_.m[2][1] = frontVec_.y;
	billboard_.m[2][2] = frontVec_.z;
#pragma endregion

	//	行列初期化
	billboardY_.Identity();

#pragma region Y軸ビルボード
	billboardY_.m[0][0] = rightVec_.x;
	billboardY_.m[0][1] = rightVec_.y;
	billboardY_.m[0][2] = rightVec_.z;
	Vector3D billYvecZ = rightVec_.cross(up_);
	billboardY_.m[2][0] = billYvecZ.x;
	billboardY_.m[2][1] = billYvecZ.y;
	billboardY_.m[2][2] = billYvecZ.z;
#pragma endregion
}

void ICamera::CalcDirectionVec()
{
	frontVec_ = target_ - eye_;
	disEyeTarget_ = frontVec_.GetLength();
	//	前方方向ベクトル
	frontVec_.Normalize();

	//	右方向ベクトル
	rightVec_ = up_.cross(frontVec_).GetNormalize();

	//	下方向ベクトル
	downVec_ = rightVec_.cross(frontVec_).GetNormalize();
}

void ICamera::ShakeUpdate()
{
	if (isShaking_ == false) return;

	shookTarget_ = target_ + move_.x * rightVec_ + move_.z * downVec_;

	shookEye_ = eye_ + move_.x * rightVec_ + move_.z * downVec_;
}

void ICamera::MatUpdate()
{
	if (isShaking_ == true) {
		matView_ = MyMath::LookAtLH(shookEye_, shookTarget_, up_);
	}
	else {
		matView_ = MyMath::LookAtLH(eye_, target_, up_);
	}
}

void ICamera::Update()
{
	//	方向ベクトル
	CalcDirectionVec();

	//	ビルボード
	CalcBillboard();

	ShakeUpdate();

	MatUpdate();
}

void ICamera::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	if (!imgui->TreeNode("Info")) return;

	imgui->Text("Front  : (%.2f, %.2f, %.2f)", frontVec_.x, frontVec_.y, frontVec_.z);
	imgui->Text("Right  : (%.2f, %.2f, %.2f)", rightVec_.x, rightVec_.y, rightVec_.z);
	imgui->Text("Down   : (%.2f, %.2f, %.2f)", downVec_.x, downVec_.y, downVec_.z);

	ImGuiInfo();

	imgui->TreePop();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

float ICamera::GetDisEyeTarget()
{
	return disEyeTarget_;
}

const Vector3D& ICamera::GetEye()
{
	return eye_;
}

const Vector3D& ICamera::GetTarget()
{
	return target_;
}

const Vector3D& ICamera::GetUp()
{
	return up_;
}

const Vector3D& ICamera::GetFrontVec()
{
	return frontVec_;
}

const Vector3D& ICamera::GetRightVec()
{
	return rightVec_;
}

const Vector3D& ICamera::GetDownVec()
{
	return downVec_;
}

const Matrix& ICamera::GetView()
{
	return matView_;
}

const Matrix& ICamera::GetProjection()
{
	return matProj_;
}

const Matrix& ICamera::GetBillboard()
{
	return billboard_;
}

const Matrix& ICamera::GetBillboardY()
{
	return billboardY_;
}

void ICamera::SetShake(float min, float max)
{
	isShaking_ = true;
	move_.x = GetRand(min, max);
	move_.z = GetRand(min, max);
}

void ICamera::StopShake()
{
	isShaking_ = false;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void ICamera::SetTarget(const Vector3D& t)
{
	target_ = t;
}

void ICamera::SetEye(const Vector3D& e)
{
	eye_ = e;
}

void ICamera::SetUp(const Vector3D& up)
{
	up_ = up;
}

void ICamera::SetProjectionMatrix(int32_t width, int32_t height, float fovY)
{
	matProj_ = MyMath::PerspectiveFovLH(width, height, fovY, 0.1f, 1000.0f);
}

void ICamera::SetProjMatrix(const Matrix& matProj)
{
	matProj_ = matProj;
}
