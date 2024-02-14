#include "PlayerWalkState.h"
#include "PlayerAvoidState.h"
#include "PlayerIdleState.h"
#include "Player.h"
#include "InputManager.h"

using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PlayerWalkState::Initialize()
{
	sPlayer_->SetAnimationIdx("Run");
	sPlayer_->SetAnimationTimer(0);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PlayerWalkState::Update()
{
	SpdUpdate();

	SetNextState();
}

void PlayerWalkState::SpdUpdate()
{
	float spd = sPlayer_->GetSpd();

	//	現在のスピード計算
	//	現在のスピードがマックスより遅かったら加速
	if (sPlayer_->GetNowMaxSpd() > spd)
	{
		spd += sPlayer_->GetAcc() + sPlayer_->GetAccDelta() * sPlayer_->GetAccRate();
	}
	//	速かったら減速
	else
	{
		spd = mMax(spd - sPlayer_->GetAcc(), sPlayer_->GetNowMaxSpd());
	}

	sPlayer_->SetSpd(spd);

	if (sPlayer_->GetIsRunning() == TRUE)
	{
		sPlayer_->SetAnimationIdx("Running");
	}
	else
	{
		sPlayer_->SetAnimationIdx("Walking");
	}
}

void PlayerWalkState::SetNextState()
{
	//	avoid
	if (sPlayer_->GetIsAvoid() == TRUE) {
		std::unique_ptr<PlayerMoveState> next_ = std::make_unique<PlayerAvoidState>();
		sPlayer_->SetMoveState(next_);
	}

	//	idle
	else if (sPlayer_->GetIsMoving() == FALSE) {
		std::unique_ptr<PlayerMoveState> next_ = std::make_unique<PlayerIdleState>();
		sPlayer_->SetMoveState(next_);
		//	走っていない
		sPlayer_->SetIsRunning(FALSE);
	}
}
