#include "DeadParticleEmitter.h"
#include "EmitterSpdAnimation.h"
#include "EmitterFadeAnimation.h"
#include "ParticleResource.h"
#include "EmitterScaleAnimation.h"
#include "EmitterSphereType.h"

void DeadParticleEmitter::SetScaleComponent()
{
	EmitterScaleAnimation* scale = emitter_->AddComponent<EmitterScaleAnimation>();
	std::unique_ptr<ParticleSameRandValue> v = std::make_unique<ParticleSameRandValue>();
	v->SetMinValue(addScaleMin_);
	v->SetMaxValue(addScaleMax_);
	std::unique_ptr<ParticleValue> v1 = std::move(v);
	scale->SetAddValue(v1);
	v1 = std::make_unique<ParticleValue>();
	v1->SetValue(startScale_);
	scale->SetValue(v1);
}

void DeadParticleEmitter::SetPosComponent()
{
	std::unique_ptr<EmitterType> type = std::make_unique<EmitterSphereType>();
	emitter_->SetEmitterType(type);
}

void DeadParticleEmitter::SetResouceComponent()
{
	ParticleResource* resource = emitter_->AddComponent<ParticleResource>();
	resource->SetResourceName("particle.png");
}

std::unique_ptr<ParticleEmitter>& DeadParticleEmitter::GetEmitter()
{
	emitter_ = std::make_unique<ParticleEmitter>();
	emitter_->SetBlendMord(Blend::ADD_BLEND);
	emitter_->SetRate(rate_);

	SetScaleComponent();
	SetPosComponent();

	emitter_->AddComponent<EmitterFadeAnimation>();
	emitter_->AddComponent<EmitterSpdAnimation>();

	SetResouceComponent();

	return emitter_;
}
