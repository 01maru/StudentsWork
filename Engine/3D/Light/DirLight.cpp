#include "DirLight.h"
#include "LightCamera.h"
#include "CameraManager.h"

void DirLight::SetLightDir(const Vector3D& dir)
{
	dir_ = dir;
	dir_.Normalize();
}

void DirLight::SetShadow(bool shadowflag)
{
	//	dirtyflagがoffだったら
	if (shadowing_ == shadowflag) return;

	shadowing_ = shadowflag;

	if (shadowing_) {
		std::unique_ptr<ICamera> lightCamera = std::make_unique<LightCamera>();
		lightCamera->Initialize(center_ + dir_ * 50.0f, center_, Vector3D{ 0.0f,1.0f,0.0f });
		CameraManager::GetInstance()->SetLightCamera(lightCamera);
	}
}
