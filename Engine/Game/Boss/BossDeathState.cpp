#include "BossDeathState.h"
#include "Boss.h"
#include "Easing.h"

#include "ParticleManager.h"
#include "DeadParticleEmitter.h"

using namespace Easing;

void BossDeathState::Initialize()
{
	const int32_t animationTime = 150;
	counter_.Initialize(animationTime, true, true);
	counter_.StartCount();
	DeadParticleEmitter emitter;
	emitter_ = ParticleManager::GetInstance()->AddEmitter(emitter.GetEmitter());
	emitter_->SetPosition(sBoss_->GetPosition());
}

void BossDeathState::Update()
{
	counter_.Update();

	float startScale = 1.0f;
	float endScale = 0.0f;
	float scale = EaseInBack(startScale, endScale, counter_.GetCountPerMaxCount());
	sBoss_->SetScale({ scale,scale,scale });

	if (counter_.GetIsActive() == false) {
		sBoss_->SetIsHide(true);

		if (emitter_ != nullptr) {
			emitter_->SetIsDead(true);
			emitter_ = nullptr;
		}
	}
}
