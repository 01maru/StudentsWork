#include "BossJumpAtState.h"
#include "Boss.h"
#include "Player.h"
#include "BossIdleState.h"
#include "Easing.h"

using namespace Easing;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void BossJumpAtState::Initialize()
{
	//	タイマー初期化
	timer_.Initialize(jumpStartTime_, TRUE);
	timer_.StartCount();
	animeTimer_.Initialize(animeTime_, TRUE);
	animeTimer_.StartCount();

	//	アニメーション設定
	sBoss_->GetAnimation()->SetAnimeName("JumpAttack");

	jumping_ = FALSE;
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void BossJumpAtState::Update()
{
	animeTimer_.Update();
	timer_.Update();

	//	アニメーション更新
	sBoss_->GetAnimation()->SetAnimeTimer(static_cast<float>(animeTimer_.GetFrameCount()));

	BefJumpUpdate();

	JumpUpdate();
}

void BossJumpAtState::BefJumpUpdate()
{
	if (jumping_ == TRUE) return;

	sBoss_->RotationUpdate();

	//	着地点確定
	if (timer_.GetIsActive() == FALSE)
	{
		startPos_ = sBoss_->GetPosition();
		endPos_ = sBoss_->GetPlayerPtr()->GetPosition();

		jumping_ = TRUE;

		timer_.Initialize(animeTime_ - jumpStartTime_, true);
		timer_.StartCount();
	}
}

void BossJumpAtState::JumpUpdate()
{
	if (jumping_ == FALSE) return;

	int32_t nowT = timer_.GetFrameCount();
	float t = nowT / static_cast<float>(jumpingTime_);
	t = MyMath::mClamp(GetNormalizedMinVal(), GetNormalizedMaxVal(), t);

	Vector3D pos = EaseInOut(startPos_, endPos_, t, Single);

	sBoss_->SetPosition(pos);

	//	次のステートへ
	if (timer_.GetIsActive() == FALSE)
	{
		std::unique_ptr<BossState> next_ = std::make_unique<BossIdleState>();
		sBoss_->SetCurrentState(next_);
	}
}
