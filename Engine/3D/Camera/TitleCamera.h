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

	
	Vector3D menuTarget_;
	Vector3D optionTarget_;
	FrameCounter counter_;
};

