#pragma once
#include "EmitterType.h"

class EmitterConeType :public EmitterType
{
public:
	void Update(Particle* particle) override;

private:
	float angle_ = 20.0f;
	float radius_ = 0.1f;
};

