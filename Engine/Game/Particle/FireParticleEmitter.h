#pragma once
#include "ParticleEmitter.h"

class FireParticleEmitter
{
private:
	std::unique_ptr<ParticleEmitter> emitter_;
	int32_t rate_ = 3;
	float addScaleMin_ = -0.2f;
	float addScaleMax_ = -0.1f;
	float endFade_ = 0.2f;
	Vector3D startScale_ = { 0.2f, 0.2f, 0.2f };

private:
	void SetScaleComponent();
	void SetPosComponent();
	void SetRotComponent();
	void SetResouceComponent();
public:
	std::unique_ptr<ParticleEmitter>& GetEmitter();
};

