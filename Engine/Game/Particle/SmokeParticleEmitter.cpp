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

using namespace MNE;
using namespace MyMath;

void SmokeParticleEmitter::SetScaleComponent()
{
	EmitterScaleAnimation* scale = emitter_->AddComponent<EmitterScaleAnimation>();
	std::unique_ptr<ParticleSameRandValue> v = std::make_unique<ParticleSameRandValue>();
	v->SetMinValue(addScaleMin_);
	v->SetMaxValue(addScaleMax_);
	std::unique_ptr<ParticleValue> v1 = std::move(v);
	scale->SetAddValue(v1);
	v1 = std::make_unique<ParticleValue>();
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
	float rotMin = 0.0f;
	float rotMax = MyMath::PIx2;
	randV->SetMinValue(rotMin);
	randV->SetMaxValue(rotMax);
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
	emitter_->SetBlendMord(MNE::Blend::ADD_BLEND);
	emitter_->SetRate(rate_);

	SetScaleComponent();
	SetRotComponent();
	SetPosComponent();

	EmitterColor* color = emitter_->AddComponent<EmitterColor>();
	Vector3D grayColor(0.7f, 0.7f, 0.7f);
	color->SetColor(grayColor);

	emitter_->AddComponent<EmitterFadeAnimation>();
	emitter_->AddComponent<EmitterSpdAnimation>();

	SetResouceComponent();

	return emitter_;
}
