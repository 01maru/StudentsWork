#pragma once
#include "ICamera.h"
#include "FrameCounter.h"

class GameOverCamera :public MNE::ICamera
{
public:
	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	void Update() override;

private:
	int32_t moveTime_ = 50;
	FrameCounter counter_;
	//	開始位置
	Vector3D eyeStart_;
	//	終了位置
	Vector3D eyeEnd_;
	//	制御点
	Vector3D controlPoint_;

	int32_t pow1 = 5;

	void ImGuiInfo() override;

public:
	bool GetEndMove();
};

