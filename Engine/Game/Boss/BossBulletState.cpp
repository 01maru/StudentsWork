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
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
		bullet->Initialize();
		bullet->SetLifeTime(bulletLifeTime_);
		bullet->SetSpd(bulletSpd_);

		Vector3D offset(0.0f, 3.0f, 0.0f);
		Vector3D pos = sBoss_->GetPosition() + offset;
		Vector3D moveVec = sBoss_->GetPlayerPtr()->GetCenterPos() - pos;
		moveVec.Normalize();
		bullet->SetMoveVec(moveVec);
		bullet->SetModel(ModelManager::GetInstance()->GetModel("bullet"));

		bullet->SetPosition(pos);
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
