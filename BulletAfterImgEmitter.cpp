#include "BulletAfterImgEmitter.h"
#include "EmitterScaleAnimation.h"
#include "EmitterColor.h"
#include "ParticleResource.h"
#include "EmitterSpdAnimation.h"
#include "EmitterLifeTime.h"

void BulletAfterImgEmitter::SetScaleComponent()
{
	float scaleValue = 0.5f;

	EmitterScaleAnimation* scale = emitter_->AddComponent<EmitterScaleAnimation>();
	std::unique_ptr<ParticleValue> v1 = std::make_unique<ParticleValue>();
	v1->SetValue(Vector3D(-scaleValue, -scaleValue, -scaleValue));
	scale->SetAddValue(v1);
	v1 = std::make_unique<ParticleValue>();
	v1->SetValue(Vector3D(scaleValue, scaleValue, scaleValue));
	scale->SetValue(v1);
}

void BulletAfterImgEmitter::SetPosComponent()
{
	std::unique_ptr<EmitterType> type = std::make_unique<EmitterType>();
	emitter_->SetEmitterType(type);
}

void BulletAfterImgEmitter::SetResouceComponent()
{
	ParticleResource* resource = emitter_->AddComponent<ParticleResource>();
	resource->SetResourceName("particle2.png");
}

std::unique_ptr<ParticleEmitter>& BulletAfterImgEmitter::GetEmitter()
{
	emitter_ = std::make_unique<ParticleEmitter>();
	emitter_->SetBlendMord(Blend::ALPHA_BLEND);
	emitter_->SetRate(4);

	SetScaleComponent();
	SetPosComponent();

	EmitterColor* color = emitter_->AddComponent<EmitterColor>();
	color->SetColor(Vector3D(0.8f, 0.3f, 0.1f));
	EmitterSpdAnimation* spd = emitter_->AddComponent<EmitterSpdAnimation>();
	spd->SetSpd(spd_);
	EmitterLifeTime* lifetime = emitter_->AddComponent<EmitterLifeTime>();
	lifetime->SetLifeTime(10);

	SetResouceComponent();

	return emitter_;
}
