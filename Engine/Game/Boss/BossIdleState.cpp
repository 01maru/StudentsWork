#include "BossIdleState.h"
#include "Boss.h"
#include "BossDeathState.h"

void BossIdleState::Update()
{
	if (sBoss_->GetIsAlive() == false) {
		std::unique_ptr<EnemyState> next_ = std::make_unique<BossDeathState>();
		sBoss_->SetCurrentState(next_);
	}
}
