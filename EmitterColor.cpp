#include "EmitterColor.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

void EmitterColor::Initialize(Particle* particle)
{
	if (parent_->GetIsObj())
	{
		ObjectParticle* obj = particle->GetComponent<ObjectParticle>();

		obj->SetColor(color_);
	}
	else
	{
		SpriteParticle* sprite = particle->GetComponent<SpriteParticle>();

		sprite->SetColor(color_);
	}
}

void EmitterColor::SetColor(const Vector3D& color)
{
	color_ = color;
}
