#pragma once
#include "ParticleEmitter.h"

class BulletHitEmitter
{
private:
	std::unique_ptr<MNE::ParticleEmitter> emitter_;
	int32_t rate_ = 3;
	int32_t lifeTime_ = 20;
	float addScaleMin_ = 0.1f;
	float addScaleMax_ = 0.3f;
private:
	void SetScaleComponent();
	void SetPosComponent();
	void SetResourceComponent();
public:
	std::unique_ptr<MNE::ParticleEmitter>& GetEmitter();
};

