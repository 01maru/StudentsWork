#include "PlayerIdleState.h"
#include "Player.h"
#include "PlayerAvoidState.h"
#include "PlayerWalkState.h"
#include "InputManager.h"
#include <memory>

using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PlayerIdleState::Initialize()
{
	sPlayer_->SetAnimationIdx("Idol");
	sPlayer_->SetAnimationTimer(0);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PlayerIdleState::Update()
{
	float spd = sPlayer_->GetSpd();
	spd = mMax(spd - sPlayer_->GetDecel(), 0.0f);
	sPlayer_->SetSpd(spd);

	//	avoid
	if (sPlayer_->GetIsAvoid() == true) {
		std::unique_ptr<PlayerMoveState> next_ = std::make_unique<PlayerAvoidState>();
		sPlayer_->SetMoveState(next_);
	}

	//	walk
	else if (sPlayer_->GetIsMoving() == true) {
		std::unique_ptr<PlayerMoveState> next_ = std::make_unique<PlayerWalkState>();
		sPlayer_->SetMoveState(next_);
	}
}
