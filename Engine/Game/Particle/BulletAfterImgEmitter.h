#pragma once
#include "ParticleEmitter.h"

class BulletAfterImgEmitter
{
private:
	std::unique_ptr<MNE::ParticleEmitter> emitter_;
	float spd_ = 0.5f;
	int32_t rate_ = 4;
	int32_t lifeTime_ = 10;
private:
	void SetScaleComponent();
	void SetPosComponent();
	void SetResouceComponent();
public:
	std::unique_ptr<MNE::ParticleEmitter>& GetEmitter();
};

