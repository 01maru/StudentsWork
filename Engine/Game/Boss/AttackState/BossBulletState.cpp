#include "BossBulletState.h"
#include "Boss.h"
#include "BossIdleState.h"
#include "ModelManager.h"

#include "Player.h"

using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void BossBulletState::Initialize()
{
	rate_.Initialize(rateTime_, true);
	rate_.StartCount();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void BossBulletState::Update()
{
	sBoss_->RotationUpdate();

	rate_.Update();

	if (rate_.GetIsActive() == FALSE) {

		//	弾生成
		EnemyBulletInfo bullet;
		bullet.type_ = NormalStone;
		bullet.lifeTime_ = bulletLifeTime_;
		bullet.spd_ = bulletSpd_;

		Vector3D moveVec = sBoss_->GetPlayerPtr()->GetCenterPos() - sBoss_->GetShotPoint();
		moveVec.Normalize();
		bullet.moveVec_ = moveVec;

		bullet.pos_ = sBoss_->GetShotPoint();
		sBoss_->AddBullet(bullet);

		//	弾をすべて撃ったら
		if (++bulletNum_ >= bulletMaxNum_) {
			//	終了
			std::unique_ptr<BossState> next_ = std::make_unique<BossIdleState>();
			sBoss_->SetCurrentState(next_);
		}
		else {
			rate_.StartCount();
		}
	}
}
