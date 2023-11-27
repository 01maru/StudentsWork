#include "BossDeathState.h"
#include "Boss.h"
#include "Easing.h"

#include "ParticleManager.h"
#include "DeadParticleEmitter.h"

void BossDeathState::Initialize()
{
	counter_.Initialize(150, true, true);
	counter_.StartCount();
	DeadParticleEmitter emitter;
	emitter_ = ParticleManager::GetInstance()->AddEmitter(emitter.GetEmitter());
	emitter_->SetPosition(sBoss_->GetPosition());
}

void BossDeathState::Update()
{
	counter_.Update();

	float scale = Easing::EaseInBack(1.0f, 0.0f, counter_.GetCountPerMaxCount());
	sBoss_->SetScale({ scale,scale,scale });

	if (counter_.GetIsActive() == false) {
		sBoss_->SetIsHide(true);

		if (emitter_ != nullptr) {
			emitter_->SetIsDead(true);
			emitter_ = nullptr;
		}
	}
}
