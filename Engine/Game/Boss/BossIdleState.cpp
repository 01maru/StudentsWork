#include "BossIdleState.h"
#include "Boss.h"
#include "BossDeathState.h"

#include "BossBulletState.h"
#include "BossWayBullets.h"
#include "BossJumpAtState.h"

using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void BossIdleState::Initialize()
{
	//	タイマー初期化
	timer_.Initialize(idleTime_, TRUE);
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
	SetStateForSpecificSituation();

	timer_.Update();

	float dis = sBoss_->RotationUpdate();
	const float MIN_DIS = 10.0f;

	if (dis > MIN_DIS) {
		Vector3D pos = sBoss_->GetPosition();
		Vector3D dir;
		dir.x = -sBoss_->GetFrontVec().x;
		dir.z = -sBoss_->GetFrontVec().z;

		pos += dir * spd_;
		sBoss_->SetPosition(pos);
	}

	if (timer_.GetIsActive() == FALSE) {
		int rad = rand();
		rad = rad % StateNum;
		if (rad == BulletState) {
			std::unique_ptr<BossState> next_ = std::make_unique<BossBulletState>();
			sBoss_->SetCurrentState(next_);
		}
		else if (rad == WayBulletsState) {
			std::unique_ptr<BossState> next_ = std::make_unique<BossWayBullets>();
			sBoss_->SetCurrentState(next_);
		}
		sBoss_->GetAnimation()->SetAutoPlay(FALSE);
		sBoss_->GetAnimation()->ResetAnimeTimer();
	}
}
