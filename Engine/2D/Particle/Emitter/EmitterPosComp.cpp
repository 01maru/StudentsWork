#include "EmitterPosComp.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

void EmitterPosComp::Initialize(Particle* particle)
{
	if (parent_->GetIsObj())
	{
		ObjectParticle* obj = particle->GetComponent<ObjectParticle>();

		Vector3D pos = obj->GetPosition();
		pos += add_->GetValue();
		obj->SetPosition(pos);
	}
	else
	{
		SpriteParticle* sprite = particle->GetComponent<SpriteParticle>();

		Vector3D pos = sprite->GetPosition();
		pos += add_->GetValue();
		sprite->SetPosition(pos);
	}
}

void EmitterPosComp::SetAddValue(std::unique_ptr<ParticleValue>& v)
{
	add_ = std::move(v);
}
