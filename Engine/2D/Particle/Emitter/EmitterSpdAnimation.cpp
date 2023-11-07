#include "EmitterSpdAnimation.h"
#include "ParticleSpdAnimation.h"
#include "ParticleEmitter.h"

void EmitterSpdAnimation::Initialize(Particle* particle)
{
	if (parent_->GetIsObj())
	{
	}
	else
	{
		ParticleSpdAnimation* spd = particle->AddComponent<ParticleSpdAnimation>();
		spd->SetSpd(spd_);
	}
}
