#include "ParticleScaleAnimation.h"
#include "Particle.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"
#include "Easing.h"

using namespace Easing;

void SpriteParticleScaleAnimation::Update()
{
	SpriteParticle* sprite = parent_->GetComponent<SpriteParticle>();

	float scale = lerp(start_, end_, parent_->GetTimerPerTime());
	sprite->SetScale(scale);
}

void SpriteParticleScaleAnimation::SetStartScale(float scale)
{
	start_ = scale;
}

void SpriteParticleScaleAnimation::SetEndScale(float scale)
{
	end_ = scale;
}

void ObjectParticleScaleAnimation::Update()
{
	ObjectParticle* obj = parent_->GetComponent<ObjectParticle>();

	Vector3D scale = lerp(start_, end_, parent_->GetTimerPerTime());
	obj->SetScale(scale);
}

void ObjectParticleScaleAnimation::SetStartScale(const Vector3D& scale)
{
	start_ = scale;
}

void ObjectParticleScaleAnimation::SetEndScale(const Vector3D& scale)
{
	end_ = scale;
}
