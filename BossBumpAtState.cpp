#include "BossBumpAtState.h"
#include "BossIdleState.h"
#include "Boss.h"
#include "Player.h"

using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void BossBumpAtState::Initialize()
{
	objMaxNum_ = static_cast<int32_t>(length_ / scale_);
	objNum_ = 0;

	dir_ = sBoss_->GetPlayerPtr()->GetCenterPos() - sBoss_->GetPosition();
	dir_.y = 0.0f;
	dir_.Normalize();

	//	プレイヤーと敵の位置で長さ変更
	Vector2D center;

}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void BossBumpAtState::Update()
{
	BeamInfo info;

	rate_.Update();

	if (rate_.GetIsActive() == FALSE) {

		//	弾生成
		EnemyBulletInfo bullet;
		bullet.type_ = BumpStone;
		bullet.lifeTime_ = bumpLifeTime_;
		//bullet.spd_ = bulletSpd_;
		bullet.scale_ = Vector3D(scale_, height_, scale_);

		Vector3D moveVec = Vector3D(0, 1, 0);
		bullet.moveVec_ = moveVec;

		Vector3D pos = sBoss_->GetPosition();
		pos += dir_ * (scale_ / 2.0f);
		pos += dir_ * scale_ * static_cast<float>(objNum_);
		bullet.pos_ = pos;
		sBoss_->AddBullet(bullet);

		//	弾をすべて撃ったら
		if (++objNum_ >= objMaxNum_) {
			//	終了
			std::unique_ptr<BossState> next_ = std::make_unique<BossIdleState>();
			sBoss_->SetCurrentState(next_);
		}
		else {
			rate_.StartCount();
		}
	}
}
