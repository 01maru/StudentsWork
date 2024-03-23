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
	dir_ = sBoss_->GetPlayerPtr()->GetCenterPos() - sBoss_->GetPosition();
	dir_.y = 0.0f;
	dir_.Normalize();

	//	プレイヤーと敵の位置で長さ変更
	Vector2D center;
	float r = 80.0f;
	Vector2D startPos = { sBoss_->GetPosition().x,sBoss_->GetPosition().z };
	Vector2D dir(dir_.x, dir_.z);
	dir.Normalize();

	//	距離計算
	length_ = GetCircleToLineLen(center, r, startPos, dir);
	//	距離から生成するオブジェクト数計算
	objMaxNum_ = static_cast<int32_t>(length_ / sBoss_->GetBumpScale());
	objNum_ = 0;
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
		bullet.lifeTime_ = sBoss_->GetBumpLifeTime();
		float scale = sBoss_->GetBumpScale();
		bullet.scale_ = Vector3D(scale, sBoss_->GetBumpHeight(), scale);

		Vector3D moveVec = Vector3D(0, 1, 0);
		bullet.moveVec_ = moveVec;

		Vector3D pos = sBoss_->GetPosition();
		pos += dir_ * (scale / 2.0f);
		pos += dir_ * scale * static_cast<float>(objNum_);
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
