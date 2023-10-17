#include "TitleCamera.h"

void TitleCamera::Initialize(const Vector3D& frontVec, const Vector3D& center, float dis)
{
	target_ = center;
	disEyeTarget_ = dis;

	Vector3D front = frontVec;
	front.Normalize();

	eye_ = target_ - disEyeTarget_ * front;

	MatUpdate();

	CalcDirectionVec();
}

void TitleCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	eye_ = eye;
	target_ = target;
	up_ = up;

	MatUpdate();

	CalcDirectionVec();
}

void TitleCamera::ImGuiInfo()
{
}

void TitleCamera::Update()
{
	CalcDirectionVec();

	CalcBillboard();

	MatUpdate();
}