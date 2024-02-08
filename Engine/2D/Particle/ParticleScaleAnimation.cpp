#include "ParticleScaleAnimation.h"
#include "Particle.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"
#include "Easing.h"

using namespace Easing;

void MNE::SpriteParticleScaleAnimation::Update()
{
	MNE::SpriteParticle* sprite = parent_->GetComponent<MNE::SpriteParticle>();

	float scale = lerp(start_, end_, parent_->GetTimerPerTime());
	sprite->SetScale(scale);
}

void MNE::SpriteParticleScaleAnimation::SetStartScale(float scale)
{
	start_ = scale;
}

void MNE::SpriteParticleScaleAnimation::SetEndScale(float scale)
{
	end_ = scale;
}

void MNE::ObjectParticleScaleAnimation::Update()
{
	MNE::ObjectParticle* obj = parent_->GetComponent<MNE::ObjectParticle>();

	MyMath::Vector3D scale = lerp(start_, end_, parent_->GetTimerPerTime());
	obj->SetScale(scale);
}

void MNE::ObjectParticleScaleAnimation::SetStartScale(const MyMath::Vector3D& scale)
{
	start_ = scale;
}

void MNE::ObjectParticleScaleAnimation::SetEndScale(const MyMath::Vector3D& scale)
{
	end_ = scale;
}
