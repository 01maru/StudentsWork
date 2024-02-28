#pragma once
#include "ParticleEmitter.h"

class PlayerLandEffect
{
private:
	std::unique_ptr<MNE::ParticleEmitter> emitter_;

	int32_t rate_ = 1;
	int32_t lifeTime_ = 20;
	int32_t particleNum_ = 20;
	float spd_ = 0.05f;

	//MyMath::Vector3D scale_ = { 1,1,1 };
	//MyMath::Vector3D addScale_ = { 1,1,1 };
	float scale_ = 0.2f;
	MyMath::Vector3D color_ = { 0.7f, 0.45f, 0.26f };

private:
	void SetScaleComponent();
	void SetSpdComponent();
	void SetResourceComponent();

public:
	std::unique_ptr<MNE::ParticleEmitter>& GetEmitter();

};

