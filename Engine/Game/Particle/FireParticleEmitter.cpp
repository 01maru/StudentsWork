#include "FireParticleEmitter.h"
#include "EmitterScaleAnimation.h"
#include "ParticleResource.h"
#include "ParticleValue.h"
#include "EmitterPosComp.h"
#include "EmitterSpdAnimation.h"

void FireParticleEmitter::SetScaleComponent()
{
	EmitterScaleAnimation* scale = emitter_->AddComponent<EmitterScaleAnimation>();
	std::unique_ptr<ParticleRandValue> v = std::make_unique<ParticleRandValue>();
	v->SetMinValue(Vector3D{ 1,1,1 });
	v->SetMaxValue(Vector3D{ 5,5,5 });
	std::unique_ptr<ParticleValue> v1 = std::move(v);
	scale->SetValue(v1);
}

void FireParticleEmitter::SetPosComponent()
{
	std::unique_ptr<ParticleRandValue> addV = std::make_unique<ParticleRandValue>();
	addV->SetMinValue(Vector3D{ -5,0,-5 });
	addV->SetMaxValue(Vector3D{ 5,0,5 });
	std::unique_ptr<ParticleValue> v2 = std::move(addV);
	EmitterPosComp* pos = emitter_->AddComponent<EmitterPosComp>();
	pos->SetAddValue(v2);
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

	SetScaleComponent();

	SetPosComponent();

	emitter_->AddComponent<EmitterSpdAnimation>();

	SetResouceComponent();

	return emitter_;
}
