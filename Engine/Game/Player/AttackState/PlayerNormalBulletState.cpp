#include "PlayerNormalBulletState.h"
#include "Player.h"

#include "ModelManager.h"

using namespace MNE;
using namespace MyMath;

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

		BulletInfo bullet;
		bullet.lifeTime_ = bulletLifeTime_;
		bullet.spd_ = bulletSpd_;
		bullet.moveVec_ = sPlayer_->GetBulletFront();
		Vector3D pos = sPlayer_->GetPosition();
		pos.y = 0.3f;
		bullet.pos_ = pos;
		sPlayer_->AddBullet(bullet);
	}
}
