#include "EmitterFadeAnimation.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"
#include "ParticleEmitter.h"
#include "ParticleFadeAnimation.h"

void MNE::EmitterFadeAnimation::Initialize(MNE::Particle* particle)
{
	MNE::ParticleFadeAnimation* anime = particle->AddComponent<MNE::ParticleFadeAnimation>();
	anime->SetStartAlpha(startValue_);
	anime->SetEndAlpha(endValue_);
	anime->SetIsObj(parent_->GetIsObj());
}

void MNE::EmitterFadeAnimation::SetEndValue(float v)
{
	endValue_ = v;
}

void MNE::EmitterFadeAnimation::SetStartValue(float v)
{
	startValue_ = v;
}
