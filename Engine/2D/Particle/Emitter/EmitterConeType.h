#pragma once
#include "EmitterType.h"

class EmitterConeType :public EmitterType
{
public:
	void Update(Particle* particle) override;

private:
	Vector3D coneDir_ = { 0.0f,1.0f,0.0f };
	float angle_ = 20.0f;
	float radius_ = 0.1f;

public:
	void SetConeDir(const Vector3D& dir) { coneDir_ = dir; }
};

