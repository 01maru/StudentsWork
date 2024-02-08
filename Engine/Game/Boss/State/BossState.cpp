#include "BossState.h"
#include "Boss.h"
#include "BossDeathState.h"
#include "BossRoarState.h"

Boss* BossState::sBoss_ = nullptr;

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void BossState::SetStateForSpecificSituation()
{
	//	死亡時
	if (sBoss_->GetIsAlive() == FALSE) {
		std::unique_ptr<BossState> next_ = std::make_unique<BossDeathState>();
		sBoss_->SetCurrentState(next_);
	}
	//	HPが半分以下になった時
	else if (sBoss_->GetIsHPLessThanHalf() == TRUE && sBoss_->GetIsSecondForm() == FALSE)
	{
		sBoss_->SetIsSecondForm(TRUE);

		std::unique_ptr<BossState> next_ = std::make_unique<BossRoarState>();
		sBoss_->SetCurrentState(next_);
	}
}

void BossState::SetBoss(Boss* boss)
{
	sBoss_ = boss;
}
