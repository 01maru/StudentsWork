#pragma once
#include "ParticleEmitter.h"

class SmokeParticleEmitter
{
private:
	std::unique_ptr<MNE::ParticleEmitter> emitter_;

	int32_t rate_ = 10;
	float addScaleMin_ = 0.5f;
	float addScaleMax_ = 0.8f;
private:
	void SetScaleComponent();
	void SetPosComponent();
	void SetRotComponent();
	void SetResouceComponent();
public:
	std::unique_ptr<MNE::ParticleEmitter>& GetEmitter();
};
