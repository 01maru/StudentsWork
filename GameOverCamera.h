#pragma once
#include "ICamera.h"
#include "FrameCounter.h"

class GameOverCamera :public ICamera
{
private:
	Vector3D eyeStart_;
	Vector3D eyeEnd_;
	FrameCounter counter_;

	FrameCounter fadeCounter_;

	Vector3D controlPoint_;

	int32_t pow1 = 5;

	void ImGuiInfo() override;

public:
	GameOverCamera() {};
	~GameOverCamera() override {};

	void Initialize(const Vector3D& frontVec, const Vector3D& center, float dis) override;
	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	void Update() override;
};

