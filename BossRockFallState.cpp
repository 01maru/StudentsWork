#include "BossRockFallState.h"
#include "Boss.h"
#include "Player.h"
#include "BossIdleState.h"

using namespace MyMath;

void BossRockFallState::Initialize()
{
	timer_.Initialize(waitTime_, true);
	timer_.StartCount();
}

void BossRockFallState::Update()
{
	sBoss_->RotationUpdate();

	timer_.Update();

	if (timer_.GetIsActive() == FALSE) {

		//	弾生成
		EnemyBulletInfo bullet;
		bullet.type_ = LandStone;
		bullet.spd_ = bulletSpd_;

		Vector3D moveVec = sBoss_->GetPlayerPtr()->GetCenterPos() - sBoss_->GetShotPoint();
		moveVec.Normalize();
		bullet.moveVec_ = moveVec;

		bullet.pos_ = sBoss_->GetShotPoint();
		sBoss_->AddBullet(bullet);

		//	終了
		std::unique_ptr<BossState> next_ = std::make_unique<BossIdleState>();
		sBoss_->SetCurrentState(next_);
	}
}
