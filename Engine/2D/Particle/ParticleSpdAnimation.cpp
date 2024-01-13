#include "ParticleSpdAnimation.h"
#include "Particle.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

void MNE::ParticleSpdAnimation::Update()
{
	if (isObj_ == true) {
		MNE::ObjectParticle* obj = parent_->GetComponent<MNE::ObjectParticle>();

		Vector3D pos = obj->GetPosition();
		pos += spd_;
		obj->SetPosition(pos);
	}

	else {
		MNE::SpriteParticle* sprite = parent_->GetComponent<MNE::SpriteParticle>();

		Vector3D pos = sprite->GetPosition();
		pos += spd_;
		sprite->SetPosition(pos);
	}
}

void MNE::ParticleSpdAnimation::SetSpd(const Vector3D& spd)
{
	spd_ = spd;
}

void MNE::ParticleSpdAnimation::SetIsObj(bool isObj)
{
	isObj_ = isObj;
}
