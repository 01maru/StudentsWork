﻿#include "NormalCamera.h"

void NormalCamera::Initialize(const Vector3D& /*frontVec*/, const Vector3D& /*center*/, float /*dis*/)
{
}

void NormalCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	eye_ = eye;
	target_ = target;
	up_ = up;

	MatUpdate();

	CalcDirectionVec();

	up_ = -downVec_;

	MatUpdate();
}

void NormalCamera::Update()
{
	CalcDirectionVec();

	CalcBillboard();

	MatUpdate();
}

void NormalCamera::ImGuiInfo()
{
}
