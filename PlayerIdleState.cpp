#include "PlayerIdleState.h"
#include "Player.h"
#include "PlayerAvoidState.h"
#include "PlayerWalkState.h"
#include "InputManager.h"
#include <memory>

void PlayerIdleState::Update()
{
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
