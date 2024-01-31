#include "PlayerSlowAttack.h"
#include "Player.h"
#include "ModelManager.h"
#include "PlayerNoAttackState.h"

using namespace MNE;

void PlayerSlowAttack::Initialize()
{
	sPlayer_->SetBulletRate(rate_);
}

void PlayerSlowAttack::Update()
{
	if (sPlayer_->GetRateCountIsActive() == false) {

		//	弾生成

		BulletInfo bullet;
		bullet.lifeTime_ = bulletLifeTime_;
		bullet.spd_ = spd_;
		bullet.moveVec_ = sPlayer_->GetBulletFront();
		Vector3D pos = sPlayer_->GetPosition();
		pos.y = 0.3f;
		bullet.pos_ = pos;
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
