#pragma once
#include "ICamera.h"
#include "FrameCounter.h"

class GameOverCamera :public MNE::ICamera
{
private:
	Vector3D eyeStart_;
	Vector3D eyeEnd_;
	FrameCounter counter_;

	Vector3D controlPoint_;

	int32_t pow1 = 5;

	void ImGuiInfo() override;

public:
	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	void Update() override;

	bool StartFade();
};

