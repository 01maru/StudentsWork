#include "PlayerWalkState.h"
#include "PlayerAvoidState.h"
#include "PlayerIdleState.h"
#include "Player.h"
#include "InputManager.h"

void PlayerWalkState::Update()
{
	SpdUpdate();

	SetNextState();
}

void PlayerWalkState::SpdUpdate()
{
	float spd;
	if (sPlayer_->GetIsRunning() == true) {
		spd = sPlayer_->GetWalkSpd();
	}
	else {
		spd = sPlayer_->GetRunSpd();
	}

	//	空中にいたら
	if (sPlayer_->GetOnGround() == false) {
		spd *= sPlayer_->GetJumpingSpdDec();
	}

	sPlayer_->SetSpd(spd);
}

void PlayerWalkState::SetNextState()
{
	//	avoid
	if (sPlayer_->GetIsAvoid() == true) {
		std::unique_ptr<PlayerMoveState> next_ = std::make_unique<PlayerAvoidState>();
		sPlayer_->SetMoveState(next_);
	}

	//	idle
	else if (sPlayer_->GetIsMoving() == false) {
		std::unique_ptr<PlayerMoveState> next_ = std::make_unique<PlayerIdleState>();
		sPlayer_->SetMoveState(next_);
	}
}
