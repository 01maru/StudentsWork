#include "ParticleSpdAnimation.h"
#include "Particle.h"
#include "SpriteParticle.h"

void ParticleSpdAnimation::Update()
{
	SpriteParticle* sprite = parent_->GetComponent<SpriteParticle>();

	Vector3D pos = sprite->GetPosition();
	pos += spd_;
	sprite->SetPosition(pos);
}

void ParticleSpdAnimation::SetSpd(const Vector3D& spd)
{
	spd_ = spd;
}
