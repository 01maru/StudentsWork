#pragma once
#include "ICamera.h"

class GameCamera :public ICamera
{
private:
	bool lockOn_ = false;
private:
	void LockOnUpdate();
	void UnLockOnUpdate();

	void ImGuiInfo() override;

public:
	GameCamera() {};
	~GameCamera() override {};

	void Initialize(const Vector3D& frontVec, const Vector3D& center, float dis) override;
	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	void Update() override;
};

