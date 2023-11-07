#include "EmitterScaleAnimation.h"
#include "SpriteParticle.h"
#include "ParticleScaleAnimation.h"
#include "ParticleEmitter.h"

void EmitterScaleAnimation::Initialize(Particle* particle)
{
	if (parent_->GetIsObj())
	{
	}
	else
	{
		SpriteParticle* sprite = particle->GetComponent<SpriteParticle>();

		sprite->SetScale(value_->GetValue().x);

		if (add_ != nullptr) {
			ParticleScaleAnimation* anime = particle->AddComponent<ParticleScaleAnimation>();
			float scale = sprite->GetScale();
			anime->SetStartScale(scale);
			scale += add_->GetValue().x;
			anime->SetEndScale(scale);
		}
	}
}

void EmitterScaleAnimation::SetValue(std::unique_ptr<ParticleValue>& v)
{
	value_ = std::move(v);
}

void EmitterScaleAnimation::SetAddValue(std::unique_ptr<ParticleValue>& v)
{
	add_ = std::move(v);
}
