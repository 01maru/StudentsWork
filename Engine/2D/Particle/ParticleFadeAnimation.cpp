#include "ParticleFadeAnimation.h"
#include "Particle.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"
#include "Easing.h"

using namespace Easing;

void MNE::ParticleFadeAnimation::Update()
{
	if (isObj_ == TRUE)
	{
		ObjectParticle* obj = parent_->GetComponent<ObjectParticle>();

		float alpha = lerp(start_, end_, parent_->GetTimerPerTime());
		obj->SetAlphaColor(alpha);
	}
	else
	{
		SpriteParticle* sprite = parent_->GetComponent<SpriteParticle>();

		float alpha = lerp(start_, end_, parent_->GetTimerPerTime());
		sprite->SetAlphaColor(alpha);
	}
}

void MNE::ParticleFadeAnimation::SetIsObj(bool isObj)
{
	isObj_ = isObj;
}

void MNE::ParticleFadeAnimation::SetStartAlpha(float alpha)
{
	start_ = alpha;
}

void MNE::ParticleFadeAnimation::SetEndAlpha(float alpha)
{
	end_ = alpha;
}
