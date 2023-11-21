#include "EmitterFadeAnimation.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"
#include "ParticleEmitter.h"
#include "ParticleFadeAnimation.h"

void EmitterFadeAnimation::Initialize(Particle* particle)
{
	if (parent_->GetIsObj())
	{
		//ObjectParticle* obj = particle->GetComponent<ObjectParticle>();


		//if (add_ != nullptr) {
		ParticleFadeAnimation* anime = particle->AddComponent<ParticleFadeAnimation>();
		anime->SetStartAlpha(maxValue_);
		anime->SetEndAlpha(minValue_);
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

void EmitterFadeAnimation::SetMinValue(float v)
{
	minValue_ = v;
}

void EmitterFadeAnimation::SetMaxValue(float v)
{
	maxValue_ = v;
}
