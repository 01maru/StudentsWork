#include "ParticleSpdAnimation.h"
#include "Particle.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

void ParticleSpdAnimation::Update()
{
	if (isObj_ == true) {
		ObjectParticle* obj = parent_->GetComponent<ObjectParticle>();

		Vector3D pos = obj->GetPosition();
		pos += spd_;
		obj->SetPosition(pos);
	}

	else {
		SpriteParticle* sprite = parent_->GetComponent<SpriteParticle>();

		Vector3D pos = sprite->GetPosition();
		pos += spd_;
		sprite->SetPosition(pos);
	}
}

void ParticleSpdAnimation::SetSpd(const Vector3D& spd)
{
	spd_ = spd;
}

void ParticleSpdAnimation::SetIsObj(bool isObj)
{
	isObj_ = isObj;
}
