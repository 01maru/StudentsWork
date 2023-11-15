#include "FireParticleEmitter.h"
#include "EmitterScaleAnimation.h"
#include "ParticleResource.h"
#include "ParticleValue.h"
#include "EmitterPosComp.h"
#include "EmitterSpdAnimation.h"

std::unique_ptr<ParticleEmitter>& FireParticleEmitter::GetEmitter()
{
	emitter_ = std::make_unique<ParticleEmitter>();
	EmitterScaleAnimation* scale = emitter_->AddComponent<EmitterScaleAnimation>();
	std::unique_ptr<ParticleRandValue> v = std::make_unique<ParticleRandValue>();
	v->SetMinValue(Vector3D{ 1,1,1 });
	v->SetMaxValue(Vector3D{ 5,5,5 });
	std::unique_ptr<ParticleValue> v1 = std::move(v);
	scale->SetValue(v1);
	std::unique_ptr<ParticleRandValue> addV = std::make_unique<ParticleRandValue>();
	addV->SetMinValue(Vector3D{ -5,0,-5 });
	addV->SetMaxValue(Vector3D{ 5,0,5 });
	std::unique_ptr<ParticleValue> v2 = std::move(addV);
	//emitter_->SetPosition({-0.0f,5.0f,0.0f });
	EmitterPosComp* pos = emitter_->AddComponent<EmitterPosComp>();
	pos->SetAddValue(v2);
	emitter_->SetBlendMord(Blend::ADD_BLEND);
	emitter_->AddComponent<EmitterSpdAnimation>();

	ParticleResource* resource = emitter_->AddComponent<ParticleResource>();
	resource->SetResourceName("");

	return emitter_;
}
