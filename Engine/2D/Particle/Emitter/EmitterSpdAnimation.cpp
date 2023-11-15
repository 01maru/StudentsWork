#include "EmitterSpdAnimation.h"
#include "ParticleSpdAnimation.h"
#include "ParticleEmitter.h"
#include "ObjectParticle.h"

void EmitterSpdAnimation::Initialize(Particle* particle)
{
	ParticleSpdAnimation* spd = particle->AddComponent<ParticleSpdAnimation>();
	spd->SetIsObj(parent_->GetIsObj());
	spd->SetSpd(spd_);
}
