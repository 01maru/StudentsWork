#include "BossStartState.h"
#include "Boss.h"
#include "BossIdleState.h"

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void BossStartState::Initialize()
{
	timer_.Initialize(roarTime_, true);
	timer_.StartCount();
	sBoss_->GetAnimation()->SetAnimeName("StartRoar");
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void BossStartState::Update()
{
	//	タイマー更新
	timer_.Update();

	sBoss_->GetAnimation()->SetAnimeTimer(static_cast<float>(timer_.GetFrameCount()));

	//	次のステートへ
	if (timer_.GetIsActive() == FALSE) {
		std::unique_ptr<BossState> next_ = std::make_unique<BossIdleState>();
		sBoss_->SetCurrentState(next_);
	}
}
