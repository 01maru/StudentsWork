#pragma once
#include "ICamera.h"

class ObjCamera2D :public ICamera
{
private:
	void ImGuiInfo() override;

public:
	ObjCamera2D();
	~ObjCamera2D() override {};

	void Initialize(const Vector3D& frontVec, const Vector3D& center, float dis) override;
	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	void Update() override;
};

