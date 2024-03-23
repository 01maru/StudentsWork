#include "BossIdleState.h"
#include "Boss.h"
#include "BossDeathState.h"

using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void BossIdleState::Initialize()
{
	//	タイマー初期化
	timer_.Initialize(idleTime_, TRUE);
	timer_.StartCount();

	//	アニメーション設定
	sBoss_->GetAnimation()->SetAnimeName("Walking");
	sBoss_->GetAnimation()->SetAutoPlay(TRUE);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void BossIdleState::MoveUpdate()
{
	Vector3D pos = sBoss_->GetPosition();
	Vector3D dir;
	dir.x = -sBoss_->GetFrontVec().x;
	dir.z = -sBoss_->GetFrontVec().z;

	pos += dir * sBoss_->GetMoveSpd();
	sBoss_->SetPosition(pos);
}


#include "BossTornadoState.h"
void BossIdleState::Update()
{
	SetStateForSpecificSituation();

	timer_.Update();

	float dis = sBoss_->RotationUpdate();

	//	最低距離になったら
	if (dis < sBoss_->GetLenMin())
	{
		//	超近距離攻撃
		sBoss_->CalcPriority(TRUE);
	}

	//	距離範囲内だったら
	else
	{
		//	前方へ移動
		MoveUpdate();

		//	一定時間経過したら攻撃ステートへ
		if (timer_.GetIsActive() == FALSE) {
			//	距離計算
			float len = mMin(dis, static_cast<float>(sBoss_->GetLenMax()));
			len -= sBoss_->GetLenMin();
			float maxLen = static_cast<float>(sBoss_->GetLenMax() - sBoss_->GetLenMin());
			len /= maxLen;

			sBoss_->CalcPriority(FALSE, len);

			sBoss_->GetAnimation()->SetAutoPlay(FALSE);
			sBoss_->GetAnimation()->ResetAnimeTimer();
		}
	}
}
