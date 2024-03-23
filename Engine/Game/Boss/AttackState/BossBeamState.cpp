#include "BossBeamState.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "RayCast.h"

#include "BossIdleState.h"
#include "Boss.h"
#include "Player.h"
#include "Quaternion.h"
#include "Easing.h"

using namespace MNE;
using namespace MNE::CollAttribute;
using namespace MyMath;
using namespace Easing;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void BossBeamState::Initialize()
{
	stateTable_ = { &BossBeamState::ChargeStateUpdate,&BossBeamState::AttackStateUpdate,&BossBeamState::EndStateUpdate };

	ChargeStateInitialize();

	avoidTimer_.Initialize(sBoss_->GetBeamAvoidTime(), TRUE);
}

void BossBeamState::ChargeStateInitialize()
{
	timer_.Initialize(sBoss_->GetBeamChargeTime(), TRUE);
	timer_.StartCount();

	attacking_ = FALSE;
	nowState_ = ChargeState;
}

void BossBeamState::AttackStateInitialize()
{
	timer_.Initialize(sBoss_->GetBeamAttackTime(), TRUE);
	timer_.StartCount();

	attacking_ = TRUE;
	nowState_ = AttackState;
}

void BossBeamState::EndStateInitialize()
{
	timer_.Initialize(sBoss_->GetBeamEndTime(), TRUE);
	timer_.StartCount();

	attacking_ = FALSE;
	nowState_ = EndState;
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void BossBeamState::Update()
{
	BeamInfo info;

	timer_.Update();

	//	一定時間避けきったら攻撃終了
	if (AvoidTimerUpdate() == FALSE)
	{
		//	終了
		std::unique_ptr<BossState> next_ = std::make_unique<BossIdleState>();
		sBoss_->SetCurrentState(next_);

		info.active_ = FALSE;
	}

	//	現在のステートの更新
	(this->*stateTable_[nowState_])(info);

	RayCollisionUpdate(info);

	sBoss_->SetBeamInfo(info);
}

void BossBeamState::RayCollisionUpdate(BeamInfo& info)
{
	Vector3D pos = sBoss_->GetBeamPoint();
	Ray ray;
	ray.start = pos;
	Vector3D dirVec = sBoss_->GetPlayerPtr()->GetCenterPos() - pos;
	dirVec.Normalize();
	ray.dir = dirVec;
	RayCast rayCastHit;
	//	ビームモデルの正面方向ベクトル
	Vector3D frontVec(0, 1, 0);

	//	レイ上の一番近いオブジェクトまでビームを伸ばす
	if (CollisionManager::GetInstance()->Raycast(ray,
		COLLISION_ATTR_LANDSHAPE | COLLISION_ATTR_ALLIES, &rayCastHit))
	{
		//	レイを正しい向きに回転
		Quaternion rotQ = DirectionToDirection(frontVec, dirVec);
		info.rotMat_ = rotQ.GetRotMatrix();

		info.pos_ = pos;

		//	レイを伸ばす
		info.scale_.y = rayCastHit.distance;

		info.active_ = TRUE;

		//	当たっているのがプレイヤーだったら
		if (rayCastHit.object->GetCollider()->GetAttribute() == COLLISION_ATTR_ALLIES)
		{
			//	攻撃中だったらダメージを与える
			if (attacking_ == TRUE)
			{
				//sBoss_->GetPlayerPtr()->DecHP(1);
			}

			//	回避中タイマーカウント中だったら止める
			if (avoidTimer_.GetIsActive() == TRUE)
			{
				avoidTimer_.Initialize(sBoss_->GetBeamAvoidTime(), TRUE);
			}
		}
		else
		{
			//	回避中タイマーをカウント
			if (avoidTimer_.GetIsActive() == FALSE)
			{
				avoidTimer_.StartCount();
			}
		}
	}
	//	レイ上に何もなかったら
	else {
		//	終了
		std::unique_ptr<BossState> next_ = std::make_unique<BossIdleState>();
		sBoss_->SetCurrentState(next_);

		info.active_ = FALSE;
	}
}

bool BossBeamState::AvoidTimerUpdate()
{
	avoidTimer_.Update();

	//	設定された時間よけ続けたら
	if (avoidTimer_.GetFrameCount() == sBoss_->GetBeamAvoidTime())
	{
		//	終了
		std::unique_ptr<BossState> next_ = std::make_unique<BossIdleState>();
		sBoss_->SetCurrentState(next_);

		return FALSE;
	}

	return TRUE;
}

void BossBeamState::ChargeStateUpdate(BeamInfo& info)
{
	float t = mMin(timer_.GetCountPerMaxCount() * 3, 1.0f);
	float alpha = EaseOut(0.0f, chargeAlpha_, t, Double);
	info.color_ = Vector4D(color_, alpha);
	info.scale_ = EaseOut(Vector3D(), chargeScale_, t, Double);

	if (timer_.GetIsActive() == FALSE)
	{
		AttackStateInitialize();
	}
}

void BossBeamState::AttackStateUpdate(BeamInfo& info)
{
	float t = mMin(timer_.GetCountPerMaxCount() * 3, 1.0f);
	float alpha = EaseOut(chargeAlpha_, 1.0f, t, Double);
	info.color_ = Vector4D(color_, alpha);
	info.scale_ = EaseOut(chargeScale_, maxScale_, t, Double);

	if (timer_.GetIsActive() == FALSE)
	{
		EndStateInitialize();
	}
}

void BossBeamState::EndStateUpdate(BeamInfo& /*info*/)
{

	if (timer_.GetIsActive() == FALSE)
	{
		//	終了
		std::unique_ptr<BossState> next_ = std::make_unique<BossIdleState>();
		sBoss_->SetCurrentState(next_);
	}
}
