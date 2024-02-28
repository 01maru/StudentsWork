#include "EmitterAccComponent.h"
#include "ParticleAccComponent.h"
#include "ParticleEmitter.h"

void MNE::EmitterAccComponent::Initialize(Particle* particle)
{
	ParticleAccComponent* spd = particle->AddComponent<ParticleAccComponent>();
	spd->SetIsObj(parent_->GetIsObj());
	//EmitterType* type = parent_->GetEmitterType();
	spd->SetSpd(spd_ * particle->GetDir());

	spd->SetAcc(acc_ * particle->GetDir() + accVec_);
}

void MNE::EmitterAccComponent::SetSpd(float spd)
{
	spd_ = spd;
}

void MNE::EmitterAccComponent::SetAcc(float acc)
{
	acc_ = acc;
}

void MNE::EmitterAccComponent::SetAccVec(const MyMath::Vector3D& v)
{
	accVec_ = v;
}
