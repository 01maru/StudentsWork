#include "PlayerSlowAttack.h"
#include "Player.h"
#include "ModelManager.h"
#include "PlayerNoAttackState.h"

void PlayerSlowAttack::Initialize()
{
	sPlayer_->SetBulletRate(rate_);
}

void PlayerSlowAttack::Update()
{
	if (sPlayer_->GetRateCountIsActive() == false) {

		//	弾生成
		std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
		bullet->Initialize();
		bullet->SetLifeTime(bulletLifeTime_);
		bullet->SetSpd(spd_);
		bullet->SetMoveVec(sPlayer_->GetFrontVec());
		bullet->SetModel(ModelManager::GetInstance()->GetModel("bullet"));
		bullet->SetPosition(sPlayer_->GetPosition());
		sPlayer_->AddBullet(bullet);

		if (++bulletNum_ >= bulletMaxNum_) {
			//	終了
			std::unique_ptr<PlayerAttackState> next_ = SetNextState(SlowAttack);

			if (next_ != nullptr)
			{
				sPlayer_->SetBulletRate(sPlayer_->GetBulletRate());
				sPlayer_->StartRateCount();
				sPlayer_->SetAttackState(next_);
			}
		}
		else {
			sPlayer_->StartRateCount();
		}
	}
}
