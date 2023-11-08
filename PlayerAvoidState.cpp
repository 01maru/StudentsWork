#include "PlayerAvoidState.h"
#include "Player.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"

void PlayerAvoidState::Initialize()
{
	avoidTimer_ = 0;
	avoidTime_ = sPlayer_->GetAvoidTime();

	if (sPlayer_->GetOnGround() == true) {
		//	横移動強く
	}

	else {
		//	横移動弱く、若干上に移動
	}
}

void PlayerAvoidState::Update()
{
	SpdUpdate();

	SetNextState();
}

void PlayerAvoidState::SpdUpdate()
{
}

void PlayerAvoidState::SetNextState()
{
	if (avoidTimer_ >= avoidTime_) return;

	sPlayer_->SetIsAvoid(false);

	if (sPlayer_->GetIsMoving() == true) {
		std::unique_ptr<PlayerMoveState> next_ = std::make_unique<PlayerWalkState>();
		sPlayer_->SetMoveState(next_);
	}

	else {
		std::unique_ptr<PlayerMoveState> next_ = std::make_unique<PlayerIdleState>();
		sPlayer_->SetMoveState(next_);
	}
}
