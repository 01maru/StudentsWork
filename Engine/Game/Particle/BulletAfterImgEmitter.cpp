#include "BulletAfterImgEmitter.h"
#include "EmitterScaleAnimation.h"
#include "EmitterColor.h"
#include "ParticleResource.h"
#include "EmitterSpdAnimation.h"
#include "EmitterLifeTime.h"

using namespace MNE;
using namespace MyMath;

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

void BulletAfterImgEmitter::SetResourceComponent()
{
	ParticleResource* resource = emitter_->AddComponent<ParticleResource>();
	resource->SetResourceName("particle2.png");
}

std::unique_ptr<ParticleEmitter>& BulletAfterImgEmitter::GetEmitter()
{
	emitter_ = std::make_unique<ParticleEmitter>();
	emitter_->SetBlendMord(MNE::Blend::ALPHA_BLEND);
	emitter_->SetRate(rate_);

	SetScaleComponent();
	SetPosComponent();

	EmitterColor* color = emitter_->AddComponent<EmitterColor>();
	Vector3D orangeColor(0.8f, 0.3f, 0.1f);
	color->SetColor(orangeColor);
	EmitterSpdAnimation* spd = emitter_->AddComponent<EmitterSpdAnimation>();
	spd->SetSpd(spd_);
	EmitterLifeTime* lifetime = emitter_->AddComponent<EmitterLifeTime>();
	lifetime->SetLifeTime(lifeTime_);

	SetResourceComponent();

	return emitter_;
}
