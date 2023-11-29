#include "ParticleFadeAnimation.h"
#include "Particle.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"
#include "Easing.h"

using namespace Easing;

void ParticleFadeAnimation::Update()
{
	ObjectParticle* sprite = parent_->GetComponent<ObjectParticle>();

	float alpha = lerp(start_, end_, parent_->GetTimerPerTime());
	sprite->SetAlphaColor(alpha);
}

void ParticleFadeAnimation::SetStartAlpha(float alpha)
{
	start_ = alpha;
}

void ParticleFadeAnimation::SetEndAlpha(float alpha)
{
	end_ = alpha;
}
