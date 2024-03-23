#include "EmitterColor.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

void MNE::EmitterColor::Initialize(MNE::Particle* particle)
{
	if (parent_->GetIsObj())
	{
		MNE::ObjectParticle* obj = particle->GetComponent<MNE::ObjectParticle>();

		obj->SetColor(color_);
		obj->SetActiveBloom(activeBloom_);
	}
	else
	{
		MNE::SpriteParticle* sprite = particle->GetComponent<MNE::SpriteParticle>();

		sprite->SetColor(color_);
	}
}

void MNE::EmitterColor::SetColor(const MyMath::Vector3D& color)
{
	color_ = color;
}

void MNE::EmitterColor::SetActiveBloom(bool active)
{
	activeBloom_ = active;
}
