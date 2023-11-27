#pragma once
#include "ParticleEmitter.h"

class DeadParticleEmitter
{
private:
	std::unique_ptr<ParticleEmitter> emitter_;

private:
	void SetScaleComponent();
	void SetPosComponent();
	void SetResouceComponent();
public:
	std::unique_ptr<ParticleEmitter>& GetEmitter();
};

