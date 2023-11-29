#pragma once
#include "ParticleEmitter.h"

class BulletAfterImgEmitter
{
private:
	std::unique_ptr<ParticleEmitter> emitter_;
	float spd_ = 0.5f;
private:
	void SetScaleComponent();
	void SetPosComponent();
	void SetResouceComponent();
public:
	std::unique_ptr<ParticleEmitter>& GetEmitter();
};

