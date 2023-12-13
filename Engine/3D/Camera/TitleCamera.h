#pragma once
#include "ICamera.h"
#include "FrameCounter.h"
#include "Quaternion.h"

class TitleCamera :public ICamera
{
private:
	enum CameraMode
	{
		Menu,
		Option,
		SceneChange,
	};

public:
	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	void Update() override;

private:
	CameraMode mord_ = Menu;
	
	Quaternion startEye_;
	Quaternion endEye_;
	Quaternion startFront_;
	Quaternion endFront_;

	FrameCounter counter_;

public:
	void SetEndEye(const Vector3D& eye);
	void SetEndFront(const Vector3D& front);
};

