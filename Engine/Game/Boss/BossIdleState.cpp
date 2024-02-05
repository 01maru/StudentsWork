#include "BossIdleState.h"
#include "Boss.h"
#include "BossDeathState.h"

#include "BossMeleeState.h"
#include "BossBulletState.h"
#include "BossWayBullets.h"
#include "BossJumpAtState.h"

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void BossIdleState::Initialize()
{
	//	タイマー初期化
	timer_.Initialize(idleTime_, true);
	timer_.StartCount();

	//	アニメーション設定
	sBoss_->GetAnimation()->SetAnimeName("Walking");
	sBoss_->GetAnimation()->SetAutoPlay(TRUE);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void BossIdleState::Update()
{
	timer_.Update();
	if (sBoss_->GetIsAlive() == FALSE) {
		std::unique_ptr<BossState> next_ = std::make_unique<BossDeathState>();
		sBoss_->SetCurrentState(next_);
	}
	else {
		float dis = sBoss_->RotationUpdate();
		const float MIN_DIS = 10.0f;

		if (dis > MIN_DIS) {
			Vector3D pos = sBoss_->GetPosition();
			Vector3D dir = -sBoss_->GetFrontVec();
			dir.y = 0.0f;
			float spd = 0.1f;
			pos += dir * spd;
			sBoss_->SetPosition(pos);
		}

		if (timer_.GetIsActive() == false) {
			int rad = rand();
			rad = rad % StateNum;
			std::unique_ptr<BossState> next_ = std::make_unique<BossJumpAtState>();
			sBoss_->SetCurrentState(next_);
			//if (rad == BulletState) {
			//	std::unique_ptr<BossState> next_ = std::make_unique<BossBulletState>();
			//	sBoss_->SetCurrentState(next_);
			//}
			//else if (rad == WayBulletsState) {
			//	std::unique_ptr<BossState> next_ = std::make_unique<BossWayBullets>();
			//	sBoss_->SetCurrentState(next_);
			//}
			sBoss_->GetAnimation()->SetAutoPlay(FALSE);
			sBoss_->GetAnimation()->SetAnimeTimer(0.0f);
		}
	}
}
