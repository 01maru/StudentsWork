#include "PlayerIdleState.h"
#include "Player.h"
#include "PlayerAvoidState.h"
#include "PlayerWalkState.h"
#include "InputManager.h"
#include <memory>

void PlayerIdleState::Initialize()
{
	//sPlayer_->SetAnimationIdx(0);
	//sPlayer_->SetAnimationTimer(0);
}

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
