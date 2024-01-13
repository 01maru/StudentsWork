#include "ParticleFadeAnimation.h"
#include "Particle.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"
#include "Easing.h"

using namespace Easing;

void MNE::ParticleFadeAnimation::Update()
{
	MNE::ObjectParticle* sprite = parent_->GetComponent<MNE::ObjectParticle>();

	float alpha = lerp(start_, end_, parent_->GetTimerPerTime());
	sprite->SetAlphaColor(alpha);
}

void MNE::ParticleFadeAnimation::SetStartAlpha(float alpha)
{
	start_ = alpha;
}

void MNE::ParticleFadeAnimation::SetEndAlpha(float alpha)
{
	end_ = alpha;
}
