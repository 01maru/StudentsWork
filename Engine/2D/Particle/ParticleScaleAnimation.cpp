#include "ParticleScaleAnimation.h"
#include "Particle.h"
#include "SpriteParticle.h"
#include "Easing.h"

using namespace Easing;

void ParticleScaleAnimation::Update()
{
	SpriteParticle* sprite = parent_->GetComponent<SpriteParticle>();

	float scale = lerp(start_, end_, parent_->GetTimerPerTime());
	sprite->SetScale(scale);
}

void ParticleScaleAnimation::SetStartScale(float scale)
{
	start_ = scale;
}

void ParticleScaleAnimation::SetEndScale(float scale)
{
	end_ = scale;
}
