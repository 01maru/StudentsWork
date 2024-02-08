#include "FireParticleEmitter.h"
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

void FireParticleEmitter::SetScaleComponent()
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

void FireParticleEmitter::SetPosComponent()
{
	std::unique_ptr<EmitterType> type = std::make_unique<EmitterConeType>();
	emitter_->SetEmitterType(type);
}

void FireParticleEmitter::SetRotComponent()
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

void FireParticleEmitter::SetResourceComponent()
{
	ParticleResource* resource = emitter_->AddComponent<ParticleResource>();
	resource->SetResourceName("");
}

std::unique_ptr<ParticleEmitter>& FireParticleEmitter::GetEmitter()
{
	emitter_ = std::make_unique<ParticleEmitter>();
	emitter_->SetIsObj(true);
	emitter_->SetBlendMord(MNE::Blend::ADD_BLEND);
	emitter_->SetRate(rate_);

	SetScaleComponent();
	SetRotComponent();
	SetPosComponent();

	EmitterColor* color = emitter_->AddComponent<EmitterColor>();
	Vector3D redColor(1.0f, 0.1f, 0.1f);
	color->SetColor(redColor);

	EmitterFadeAnimation* fade = emitter_->AddComponent<EmitterFadeAnimation>();
	fade->SetEndValue(endFade_);
	emitter_->AddComponent<EmitterSpdAnimation>();

	SetResourceComponent();

	return emitter_;
}
