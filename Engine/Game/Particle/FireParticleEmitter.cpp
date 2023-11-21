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

void FireParticleEmitter::SetScaleComponent()
{
	EmitterScaleAnimation* scale = emitter_->AddComponent<EmitterScaleAnimation>();
	std::unique_ptr<ParticleSameRandValue> v = std::make_unique<ParticleSameRandValue>();
	v->SetMinValue(-0.2f);
	v->SetMaxValue(-0.1f);
	std::unique_ptr<ParticleValue> v1 = std::move(v);
	scale->SetAddValue(v1);
	v1 = std::make_unique<ParticleValue>();
	v1->SetValue(Vector3D(0.2f, 0.2f, 0.2f));
	scale->SetValue(v1);
}

void FireParticleEmitter::SetPosComponent()
{
	//std::unique_ptr<ParticleRandValue> addV = std::make_unique<ParticleRandValue>();
	//addV->SetMinValue(Vector3D{ -0.1f,0.0f,-0.1f });
	//addV->SetMaxValue(Vector3D{ 0.1f,0.0f,0.1f });
	//std::unique_ptr<ParticleValue> v2 = std::move(addV);
	//EmitterPosComp* pos = emitter_->AddComponent<EmitterPosComp>();
	//pos->SetAddValue(v2);
	std::unique_ptr<EmitterType> type = std::make_unique<EmitterConeType>();
	emitter_->SetEmitterType(type);
}

void FireParticleEmitter::SetRotComponent()
{
	std::unique_ptr<ParticleSameRandValue> randV = std::make_unique<ParticleSameRandValue>();
	randV->SetMinValue(0.0f);
	randV->SetMaxValue(MyMath::PIx2);
	std::unique_ptr<ParticleValue> v = std::move(randV);
	EmitterRotation* rot = emitter_->AddComponent<EmitterRotation>();
	rot->SetValue(v);
}

void FireParticleEmitter::SetResouceComponent()
{
	ParticleResource* resource = emitter_->AddComponent<ParticleResource>();
	resource->SetResourceName("");
}

std::unique_ptr<ParticleEmitter>& FireParticleEmitter::GetEmitter()
{
	emitter_ = std::make_unique<ParticleEmitter>();
	emitter_->SetIsObj(true);
	emitter_->SetBlendMord(Blend::ADD_BLEND);
	emitter_->SetRate(3);

	SetScaleComponent();
	SetRotComponent();
	SetPosComponent();

	EmitterColor* color = emitter_->AddComponent<EmitterColor>();
	color->SetColor(Vector3D(1.0f, 0.1f, 0.1f));

	EmitterFadeAnimation* fade = emitter_->AddComponent<EmitterFadeAnimation>();
	fade->SetMinValue(0.2f);
	emitter_->AddComponent<EmitterSpdAnimation>();

	SetResouceComponent();

	return emitter_;
}
