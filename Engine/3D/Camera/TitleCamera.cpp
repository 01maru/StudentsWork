#include "TitleCamera.h"

void TitleCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	ICamera::Initialize(eye, target, up);

	startEye_.SetVec(eye);
	endEye_.SetVec(eye);
	startFront_.SetVec(frontVec_);
	endFront_.SetVec(frontVec_);
}

void TitleCamera::Update()
{
	counter_.Update();

	//eye_ = CameraSlerp(startEye_, endEye_, counter_.GetCountPerMaxCount()).GetVector3();
	//frontVec_ = CameraSlerp(startFront_, endFront_, counter_.GetCountPerMaxCount()).GetVector3();
	//frontVec_.Normalize();

	//target_ = eye_ + frontVec_ * disEyeTarget_;

	ICamera::Update();
}

void TitleCamera::SetEndEye(const Vector3D& eye)
{
	startEye_.SetVec(eye_);
	startEye_.w = 0.0f;

	endEye_.SetVec(eye);
	endEye_.w = 0.0f;

	counter_.StartCount();
}

void TitleCamera::SetEndFront(const Vector3D& front)
{
	startFront_.SetVec(frontVec_);
	startFront_.w = 0.0f;

	endFront_.SetVec(front);
	endFront_.w = 0.0f;

	counter_.StartCount();
}
