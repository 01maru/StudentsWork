#include "EmitterType.h"
#include "ParticleEmitter.h"
#include "ObjectParticle.h"
#include "SpriteParticle.h"

void MNE::EmitterType::Update(MNE::Particle* particle)
{
	pos_ = parent_->GetPosition();

	if (parent_->GetIsObj())
	{
		MNE::ObjectParticle* obj = particle->GetComponent<MNE::ObjectParticle>();

		obj->SetPosition(pos_);
	}
	else
	{
		MNE::SpriteParticle* sprite = particle->GetComponent<MNE::SpriteParticle>();

		sprite->SetPosition(pos_);
	}
}

void MNE::EmitterType::SetParent(MNE::ParticleEmitter* parent)
{
	parent_ = parent;
}
