#include "PlayerSlideEffect.h"
#include "EmitterColor.h"
#include "ParticleResource.h"
#include "EmitterAccComponent.h"
#include "EmitterScaleAnimation.h"
#include "EmitterCircleType.h"

using namespace MNE;
using namespace MyMath;

void PlayerSlideEffect::SetScaleComponent()
{
	EmitterScaleAnimation* scale = emitter_->AddComponent<EmitterScaleAnimation>();

	MyMath::Vector3D startScale = { scale_,scale_,scale_ };
	MyMath::Vector3D addScale = { scale_,scale_,scale_ };

	std::unique_ptr<ParticleValue> v1 = std::make_unique<ParticleValue>();
	v1->SetValue(addScale);
	scale->SetAddValue(v1);
	v1 = std::make_unique<ParticleValue>();
	v1->SetValue(startScale);
	scale->SetValue(v1);
}

void PlayerSlideEffect::SetSpdComponent()
{
	EmitterAccComponent* comp = emitter_->AddComponent<EmitterAccComponent>();
	comp->SetSpd(spd_);
	float acc = spd_ / lifeTime_;
	comp->SetAcc(-acc);

	Vector3D vec(0.0f, 0.001f, 0.0f);
	comp->SetAccVec(vec);
}

void PlayerSlideEffect::SetResourceComponent()
{
	//	リソース
	ParticleResource* resource = emitter_->AddComponent<ParticleResource>();
	resource->SetResourceName("particle2.png");

	//	色
	EmitterColor* color = emitter_->AddComponent<EmitterColor>();
	color->SetColor(color_);
}

std::unique_ptr<MNE::ParticleEmitter>& PlayerSlideEffect::GetEmitter()
{
	emitter_ = std::make_unique<ParticleEmitter>();
	emitter_->SetIsObj(false);
	emitter_->SetBlendMord(Blend::ALPHA_BLEND);
	emitter_->SetRate(rate_);
	emitter_->SetLifeTime(lifeTime_);

	SetScaleComponent();

	//	エミッターの型設定
	std::unique_ptr<EmitterType> type = std::make_unique<EmitterCircleType>();
	type->SetParticleNum(particleNum_);
	emitter_->SetEmitterType(type);

	//emitter_->AddComponent<EmitterFadeAnimation>();

	SetSpdComponent();
	SetResourceComponent();

	return emitter_;
}
