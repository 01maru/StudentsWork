﻿#pragma once
#include "ICamera.h"

class GameCamera :public ICamera
{
private:
	void ImGuiInfo() override;

public:
	GameCamera() {};
	~GameCamera() override {};

	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	void Update() override;
};

