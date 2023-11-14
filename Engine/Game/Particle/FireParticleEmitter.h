#pragma once
#include "ParticleEmitter.h"

class FireParticleEmitter
{
private:
	std::unique_ptr<ParticleEmitter> emitter_;

public:
	std::unique_ptr<ParticleEmitter>& GetEmitter();
};

