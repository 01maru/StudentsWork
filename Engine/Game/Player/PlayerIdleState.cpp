#include "PlayerIdleState.h"
#include "Player.h"
#include "PlayerAvoidState.h"
#include "PlayerWalkState.h"
#include "InputManager.h"
#include <memory>

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
	//	急に止まる原因(後で修正)
	sPlayer_->SetSpd(0.0f);

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
