#include "ParticleAccComponent.h"
#include "Particle.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

void MNE::ParticleAccComponent::Update()
{
	if (isObj_ == TRUE) {
		MNE::ObjectParticle* obj = parent_->GetComponent<MNE::ObjectParticle>();

		MyMath::Vector3D pos = obj->GetPosition();
		pos += spd_;
		obj->SetPosition(pos);
	}

	else {
		MNE::SpriteParticle* sprite = parent_->GetComponent<MNE::SpriteParticle>();

		MyMath::Vector3D pos = sprite->GetPosition();
		pos += spd_;
		sprite->SetPosition(pos);
	}

	spd_ += acc_;
}

void MNE::ParticleAccComponent::SetSpd(const MyMath::Vector3D& spd)
{
	spd_ = spd;
}

void MNE::ParticleAccComponent::SetAcc(const MyMath::Vector3D& acc)
{
	acc_ = acc;
}

void MNE::ParticleAccComponent::SetIsObj(bool isObj)
{
	isObj_ = isObj;
}
