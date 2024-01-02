#pragma once
#include "ICamera.h"

class GameCamera :public ICamera
{
private:
	bool lockOn_ = false;
	bool isActive_ = true;

private:
	void LockOnUpdate();
	void UnLockOnUpdate();

	void ImGuiInfo() override;

public:
	void Initialize(const Vector3D& frontVec, const Vector3D& center, float dis) override;
	void Update() override;

	void SetIsActive(bool isActive);
};

