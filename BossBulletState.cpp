#include "BossBulletState.h"
#include "Boss.h"
#include "BossIdleState.h"
#include "ModelManager.h"

void BossBulletState::Initialize()
{
	rate_.Initialize(60, true);
	rate_.StartCount();

	bulletNum_ = 0;
	bulletMaxNum_ = 5;
}

void BossBulletState::Update()
{
	sBoss_->RotationUpdate();

	rate_.Update();

	if (rate_.GetIsActive() == false) {

		//	弾生成
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
		bullet->Initialize();
		bullet->SetLifeTime(600);
		bullet->SetSpd(1);
		bullet->SetMoveVec(-sBoss_->GetFrontVec());
		bullet->SetModel(ModelManager::GetInstance()->GetModel("bullet"));
		bullet->SetPosition(sBoss_->GetPosition());
		sBoss_->AddBullet(bullet);

		if (++bulletNum_ >= bulletMaxNum_) {
			//	終了
			std::unique_ptr<EnemyState> next_ = std::make_unique<BossIdleState>();
			sBoss_->SetCurrentState(next_);
		}
		else {
			rate_.StartCount();
		}
	}
}
