#include "BulletHitEmitter.h"
#include "EmitterColor.h"
#include "EmitterConeType.h"
#include "EmitterScaleAnimation.h"
#include "ParticleResource.h"
#include "EmitterSpdAnimation.h"
#include "EmitterFadeAnimation.h"
#include "EmitterLifeTime.h"

using namespace MNE;
using namespace MyMath;

void BulletHitEmitter::SetScaleComponent()
{
	float scaleValue = 0.1f;

	EmitterScaleAnimation* scale = emitter_->AddComponent<EmitterScaleAnimation>();
	std::unique_ptr<ParticleSameRandValue> v = std::make_unique<ParticleSameRandValue>();
	v->SetMinValue(addScaleMin_);
	v->SetMaxValue(addScaleMax_);
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

void BulletHitEmitter::SetResourceComponent()
{
	ParticleResource* resource = emitter_->AddComponent<ParticleResource>();
	resource->SetResourceName("particle2.png");
}

std::unique_ptr<MNE::ParticleEmitter>& BulletHitEmitter::GetEmitter()
{
	emitter_ = std::make_unique<ParticleEmitter>();
	emitter_->SetBlendMord(MNE::Blend::ALPHA_BLEND);
	emitter_->SetRate(rate_);

	SetScaleComponent();
	SetPosComponent();

	EmitterColor* color = emitter_->AddComponent<EmitterColor>();
	MyMath::Vector3D redColor(1.0f, 0.1f, 0.1f);
	color->SetColor(redColor);

	emitter_->AddComponent<EmitterFadeAnimation>();
	emitter_->AddComponent<EmitterSpdAnimation>();
	EmitterLifeTime* lifetime = emitter_->AddComponent<EmitterLifeTime>();
	lifetime->SetLifeTime(lifeTime_);

	SetResourceComponent();

	return emitter_;
}
