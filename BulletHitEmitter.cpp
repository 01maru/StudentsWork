#include "BulletHitEmitter.h"
#include "EmitterColor.h"
#include "EmitterConeType.h"
#include "EmitterScaleAnimation.h"
#include "ParticleResource.h"
#include "EmitterSpdAnimation.h"
#include "EmitterFadeAnimation.h"
#include "EmitterLifeTime.h"

void BulletHitEmitter::SetScaleComponent()
{
	float scaleValue = 0.1f;

	EmitterScaleAnimation* scale = emitter_->AddComponent<EmitterScaleAnimation>();
	std::unique_ptr<ParticleSameRandValue> v = std::make_unique<ParticleSameRandValue>();
	v->SetMinValue(0.1f);
	v->SetMaxValue(0.3f);
	std::unique_ptr<ParticleValue> v1 = std::move(v);
	scale->SetAddValue(v1);
	v1 = std::make_unique<ParticleValue>();
	v1->SetValue(Vector3D(scaleValue, scaleValue, scaleValue));
	scale->SetValue(v1);
}

void BulletHitEmitter::SetPosComponent()
{
	std::unique_ptr<EmitterType> type = std::make_unique<EmitterConeType>();
	emitter_->SetEmitterType(type);
}

void BulletHitEmitter::SetResouceComponent()
{
	ParticleResource* resource = emitter_->AddComponent<ParticleResource>();
	resource->SetResourceName("particle2.png");
}

std::unique_ptr<ParticleEmitter>& BulletHitEmitter::GetEmitter()
{
	emitter_ = std::make_unique<ParticleEmitter>();
	emitter_->SetBlendMord(Blend::ALPHA_BLEND);
	emitter_->SetRate(3);

	SetScaleComponent();
	SetPosComponent();

	EmitterColor* color = emitter_->AddComponent<EmitterColor>();
	color->SetColor(Vector3D(1.0f, 0.1f, 0.1f));

	emitter_->AddComponent<EmitterFadeAnimation>();
	emitter_->AddComponent<EmitterSpdAnimation>();
	EmitterLifeTime* lifetime = emitter_->AddComponent<EmitterLifeTime>();
	lifetime->SetLifeTime(20);

	SetResouceComponent();

	return emitter_;
}
