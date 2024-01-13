#include "EmitterFadeAnimation.h"
#include "SpriteParticle.h"
#include "ObjectParticle.h"
#include "ParticleEmitter.h"
#include "ParticleFadeAnimation.h"

void MNE::EmitterFadeAnimation::Initialize(MNE::Particle* particle)
{
	if (parent_->GetIsObj())
	{
		//ObjectParticle* obj = particle->GetComponent<ObjectParticle>();


		//if (add_ != nullptr) {
		MNE::ParticleFadeAnimation* anime = particle->AddComponent<MNE::ParticleFadeAnimation>();
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

void MNE::EmitterFadeAnimation::SetEndValue(float v)
{
	endValue_ = v;
}

void MNE::EmitterFadeAnimation::SetStartValue(float v)
{
	startValue_ = v;
}
