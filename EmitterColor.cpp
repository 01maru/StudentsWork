#include "EmitterColor.h"
#include "ParticleEmitter.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"

void EmitterColor::Initialize(Particle* particle)
{
	if (parent_->GetIsObj())
	{
		ObjectParticle* obj = particle->GetComponent<ObjectParticle>();

		obj->SetColor(color_);
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

void EmitterColor::SetColor(const Vector3D& color)
{
	color_ = color;
}
