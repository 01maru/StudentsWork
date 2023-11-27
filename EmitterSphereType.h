#pragma once
#include "EmitterType.h"

 class EmitterSphereType :public EmitterType
{
public:
	void Update(Particle* particle) override;

private:
	float radiusThickness_ = 0.0f;
	float radius_ = 1.0f;

public:
	void SetRadius(float radius);
	void SetRadiusThickness(float thickness);
};

