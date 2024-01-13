#include "EmitterPosComp.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

void MNE::EmitterPosComp::Initialize(MNE::Particle* particle)
{
	if (parent_->GetIsObj())
	{
		MNE::ObjectParticle* obj = particle->GetComponent<MNE::ObjectParticle>();

		Vector3D pos = obj->GetPosition();
		pos += add_->GetValue();
		obj->SetPosition(pos);
	}
	else
	{
		MNE::SpriteParticle* sprite = particle->GetComponent<MNE::SpriteParticle>();

		Vector3D pos = sprite->GetPosition();
		pos += add_->GetValue();
		sprite->SetPosition(pos);
	}
}

void MNE::EmitterPosComp::SetAddValue(std::unique_ptr<ParticleValue>& v)
{
	add_ = std::move(v);
}
