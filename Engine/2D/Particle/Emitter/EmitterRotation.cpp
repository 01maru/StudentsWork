#include "EmitterRotation.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"
#include "ParticleEmitter.h"

void EmitterRotation::Initialize(Particle* particle)
{
	if (parent_->GetIsObj())
	{
		ObjectParticle* obj = particle->GetComponent<ObjectParticle>();

		obj->SetRotation(value_->GetValue());

		//if (add_ != nullptr) {
		//	ObjectParticleScaleAnimation* anime = particle->AddComponent<ObjectParticleScaleAnimation>();
		//	Vector3D scale = obj->GetScale();
		//	anime->SetStartScale(scale);
		//	scale += add_->GetValue();
		//	anime->SetEndScale(scale);
		//}
	}
	else
	{
		//SpriteParticle* sprite = particle->GetComponent<SpriteParticle>();

		//sprite->SetScale(value_->GetValue().x);

		//if (add_ != nullptr) {
		//	SpriteParticleScaleAnimation* anime = particle->AddComponent<SpriteParticleScaleAnimation>();
		//	float scale = sprite->GetScale();
		//	anime->SetStartScale(scale);
		//	scale += add_->GetValue().x;
		//	anime->SetEndScale(scale);
		//}
	}
}

void EmitterRotation::SetValue(std::unique_ptr<ParticleValue>& v)
{
	value_ = std::move(v);
}

void EmitterRotation::SetAddValue(std::unique_ptr<ParticleValue>& v)
{
	add_ = std::move(v);
}
