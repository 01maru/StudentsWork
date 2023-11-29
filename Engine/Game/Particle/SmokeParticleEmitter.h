#pragma once
#include "ParticleEmitter.h"

class SmokeParticleEmitter
{
private:
	std::unique_ptr<ParticleEmitter> emitter_;

private:
	void SetScaleComponent();
	void SetPosComponent();
	void SetRotComponent();
	void SetResouceComponent();
public:
	std::unique_ptr<ParticleEmitter>& GetEmitter();
};
