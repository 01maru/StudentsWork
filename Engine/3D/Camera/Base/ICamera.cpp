#include "ICamera.h"
#include "ImGuiManager.h"

using namespace MyMath;

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
	disEyeTarget_ = (target_ - eye_).GetLength();
	//	前方方向ベクトル
	frontVec_ = target_ - eye_;
	frontVec_.Normalize();

	//	右方向ベクトル
	rightVec_ = up_.cross(frontVec_).GetNormalize();

	//	下方向ベクトル
	downVec_ = rightVec_.cross(frontVec_).GetNormalize();
}

void ICamera::ShakeUpdate()
{
	if (isShaking_ == false) return;

	target_ = oldTarget_ + move_.x * rightVec_ + move_.z * downVec_;

	eye_ = oldEye_ + move_.x * rightVec_ + move_.z * downVec_;
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

void ICamera::MatUpdate()
{
	matView_ = MyMath::LookAtLH(eye_, target_, up_);
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
	target_ = oldTarget_;
	eye_ = oldEye_;
}

void ICamera::SetProjectionMatrix(int32_t width, int32_t height, float fovY)
{
	matProjection_ = MyMath::PerspectiveFovLH(width, height, fovY, 0.1f, 1000.0f);
}
