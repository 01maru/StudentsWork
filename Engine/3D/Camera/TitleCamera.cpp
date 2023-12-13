#include "TitleCamera.h"

void TitleCamera::Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up)
{
	ICamera::Initialize(eye, target, up);

	menuTarget_ = target;
}

void TitleCamera::Update()
{
	counter_.Update();

	
	switch (mord_)
	{
	case TitleCamera::Menu:
		break;
	case TitleCamera::Option:
		break;
	case TitleCamera::SceneChange:
		break;
	default:
		break;
	}
}
