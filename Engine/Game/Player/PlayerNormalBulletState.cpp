#include "PlayerNormalBulletState.h"
#include "Player.h"

#include "ModelManager.h"

void PlayerNormalBulletState::Initialize()
{
	sPlayer_->SetBulletRate(sPlayer_->GetBulletRate());
}

void PlayerNormalBulletState::Update()
{
	std::unique_ptr<PlayerAttackState> next_ = SetNextState(NormalAttack);

	if (next_ != nullptr)
	{
		sPlayer_->SetAttackState(next_);
	}


	if (sPlayer_->GetRateCountIsActive() == false) {
		//	弾生成
		sPlayer_->StartRateCount();

		std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
		bullet->Initialize();
		bullet->SetLifeTime(bulletLifeTime_);
		bullet->SetSpd(bulletSpd_);
		bullet->SetMoveVec(sPlayer_->GetBulletFront());
		bullet->SetModel(ModelManager::GetInstance()->GetModel("bullet"));
		Vector3D pos = sPlayer_->GetPosition();
		pos.y = 0.3f;
		bullet->SetPosition(pos);
		sPlayer_->AddBullet(bullet);
	}
}
