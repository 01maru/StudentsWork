#include "SmokeParticleEmitter.h"
#include "EmitterScaleAnimation.h"
#include "ParticleResource.h"
#include "ParticleValue.h"
#include "EmitterPosComp.h"
#include "EmitterSpdAnimation.h"
#include "EmitterRotation.h"
#include "EmitterFadeAnimation.h"
#include "EmitterConeType.h"
#include "EmitterColor.h"
#include "MyMath.h"

void SmokeParticleEmitter::SetScaleComponent()
{
	EmitterScaleAnimation* scale = emitter_->AddComponent<EmitterScaleAnimation>();
	std::unique_ptr<ParticleSameRandValue> v = std::make_unique<ParticleSameRandValue>();
	v->SetMinValue(0.5f);
	v->SetMaxValue(0.8f);
	std::unique_ptr<ParticleValue> v1 = std::move(v);
	scale->SetAddValue(v1);
	v1 = std::make_unique<ParticleValue>();
	v1->SetValue({ 0,0,0 });
	scale->SetValue(v1);
}

void SmokeParticleEmitter::SetPosComponent()
{
	std::unique_ptr<EmitterType> type = std::make_unique<EmitterConeType>();
	emitter_->SetEmitterType(type);
}

void SmokeParticleEmitter::SetRotComponent()
{
	std::unique_ptr<ParticleSameRandValue> randV = std::make_unique<ParticleSameRandValue>();
	randV->SetMinValue(0.0f);
	randV->SetMaxValue(MyMath::PIx2);
	std::unique_ptr<ParticleValue> v = std::move(randV);
	EmitterRotation* rot = emitter_->AddComponent<EmitterRotation>();
	rot->SetValue(v);
}

void SmokeParticleEmitter::SetResouceComponent()
{
	ParticleResource* resource = emitter_->AddComponent<ParticleResource>();
	resource->SetResourceName("");
}

std::unique_ptr<ParticleEmitter>& SmokeParticleEmitter::GetEmitter()
{
	emitter_ = std::make_unique<ParticleEmitter>();
	emitter_->SetIsObj(true);
	emitter_->SetBlendMord(Blend::ADD_BLEND);
	emitter_->SetRate(10);

	SetScaleComponent();
	SetRotComponent();
	SetPosComponent();

	EmitterColor* color = emitter_->AddComponent<EmitterColor>();
	color->SetColor(Vector3D(0.7f, 0.7f, 0.7f));

	emitter_->AddComponent<EmitterFadeAnimation>();
	emitter_->AddComponent<EmitterSpdAnimation>();

	SetResouceComponent();

	return emitter_;
}
