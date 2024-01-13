#pragma once
#include "ParticleEmitter.h"

class DeadParticleEmitter
{
private:
	std::unique_ptr<MNE::ParticleEmitter> emitter_;
	int32_t rate_ = 1;
	float addScaleMin_ = -0.2f;
	float addScaleMax_ = -0.1f;
	Vector3D startScale_ = { 0.2f, 0.2f, 0.2f };
private:
	void SetScaleComponent();
	void SetPosComponent();
	void SetResouceComponent();
public:
	std::unique_ptr<MNE::ParticleEmitter>& GetEmitter();
};

