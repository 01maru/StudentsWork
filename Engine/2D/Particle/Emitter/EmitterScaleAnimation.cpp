#include "EmitterScaleAnimation.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"
#include "ParticleScaleAnimation.h"
#include "ParticleEmitter.h"

using namespace MyMath;

void MNE::EmitterScaleAnimation::Initialize(MNE::Particle* particle)
{
	if (parent_->GetIsObj())
	{
		MNE::ObjectParticle* obj = particle->GetComponent<MNE::ObjectParticle>();

		obj->SetScale(value_->GetValue());

		if (add_ != nullptr) {
			ObjectParticleScaleAnimation* anime = particle->AddComponent<ObjectParticleScaleAnimation>();
			Vector3D scale = obj->GetScale();
			anime->SetStartScale(scale);
			scale += add_->GetValue();
			anime->SetEndScale(scale);
		}
	}
	else
	{
		MNE::SpriteParticle* sprite = particle->GetComponent<MNE::SpriteParticle>();

		sprite->SetScale(value_->GetValue().x);

		if (add_ != nullptr) {
			SpriteParticleScaleAnimation* anime = particle->AddComponent<SpriteParticleScaleAnimation>();
			float scale = sprite->GetScale();
			anime->SetStartScale(scale);
			scale += add_->GetValue().x;
			anime->SetEndScale(scale);
		}
	}
}

void MNE::EmitterScaleAnimation::SetValue(std::unique_ptr<ParticleValue>& v)
{
	value_ = std::move(v);
}

void MNE::EmitterScaleAnimation::SetAddValue(std::unique_ptr<ParticleValue>& v)
{
	add_ = std::move(v);
}
