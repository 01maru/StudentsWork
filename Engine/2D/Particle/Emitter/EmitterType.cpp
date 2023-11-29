#include "EmitterType.h"
#include "ParticleEmitter.h"
#include "ObjectParticle.h"
#include "SpriteParticle.h"

void EmitterType::Update(Particle* particle)
{
	pos_ = parent_->GetPosition();

	if (parent_->GetIsObj())
	{
		ObjectParticle* obj = particle->GetComponent<ObjectParticle>();

		obj->SetPosition(pos_);
	}
	else
	{
		SpriteParticle* sprite = particle->GetComponent<SpriteParticle>();

		sprite->SetPosition(pos_);
	}
}

void EmitterType::SetParent(ParticleEmitter* parent)
{
	parent_ = parent;
}
