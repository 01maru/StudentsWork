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
		anime->SetStartAlpha(startValue_);
		anime->SetEndAlpha(endValue_);
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

void EmitterFadeAnimation::SetEndValue(float v)
{
	endValue_ = v;
}

void EmitterFadeAnimation::SetStartValue(float v)
{
	startValue_ = v;
}
