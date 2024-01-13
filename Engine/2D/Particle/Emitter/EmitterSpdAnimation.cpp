#include "EmitterSpdAnimation.h"
#include "ParticleSpdAnimation.h"
#include "ParticleEmitter.h"

void MNE::EmitterSpdAnimation::Initialize(MNE::Particle* particle)
{
	MNE::ParticleSpdAnimation* spd = particle->AddComponent<MNE::ParticleSpdAnimation>();
	spd->SetIsObj(parent_->GetIsObj());
	EmitterType* type = parent_->GetEmitterType();
	spd->SetSpd(spd_ * type->GetDir());
}
