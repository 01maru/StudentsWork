#include "PlayerAvoidState.h"
#include "Player.h"
#include "PlayerIdleState.h"
#include "PlayerWalkState.h"
#include "ImGuiManager.h"
#include "Easing.h"

using namespace MNE;

void PlayerAvoidState::Initialize()
{
	timer_.Initialize(sPlayer_->GetAvoidAccTime(), true);
	timer_.StartCount();

	acceleration_ = true;

	decTimer_ = sPlayer_->GetAvoidDecTime();

	startSpd_= sPlayer_->GetSpd();
	endSpd_ = sPlayer_->GetRunSpd();

	if (sPlayer_->GetOnGround() == true) {
		//	横移動強く
		maxSpd_ = sPlayer_->GetAvoidMaxSpd();
		sPlayer_->SetAnimationIdx("Rolling");
		sPlayer_->SetAnimationTimer(0);
	}

	else {
		//	横移動弱く、若干上に移動
	}
}

void PlayerAvoidState::Update()
{
	timer_.Update();

	SpdUpdate();
}

void PlayerAvoidState::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->Text("Acceleration : %s", acceleration_ ? "TRUE" : "FALSE");
	imgui->Text("Frame : %d", timer_.GetFrameCount());

	//imgui->InputFloat("walkSpd", walkSpd_);
	//imgui->InputFloat("runSpd", runSpd_);
	//imgui->InputFloat("jumpingSpdDec", jumpingSpdDec_);
}

void PlayerAvoidState::SpdUpdate()
{
	if (acceleration_ == true) {
		AccSpdUpdate();
	}
	else {
		DecSpdUpdate();
	}
}

void PlayerAvoidState::SetNextState()
{
	sPlayer_->SetIsAvoid(false);

	if (sPlayer_->GetIsMoving() == true) {
		std::unique_ptr<PlayerMoveState> next_ = std::make_unique<PlayerWalkState>();
		sPlayer_->SetMoveState(next_);

		sPlayer_->SetIsRunning(true);
	}

	else {
		std::unique_ptr<PlayerMoveState> next_ = std::make_unique<PlayerIdleState>();
		sPlayer_->SetMoveState(next_);
	}
}

void PlayerAvoidState::DecSpdUpdate()
{
	float spd = Easing::lerp(maxSpd_, endSpd_, timer_.GetCountPerMaxCount());
	sPlayer_->SetSpd(spd);

	if (timer_.GetIsActive() == false) {
		SetNextState();
	}
}

void PlayerAvoidState::AccSpdUpdate()
{
	float spd = Easing::EaseOut(startSpd_, maxSpd_, timer_.GetCountPerMaxCount(), 3);
	sPlayer_->SetSpd(spd);

	if (timer_.GetIsActive() == false) {
		acceleration_ = false;

		timer_.Initialize(decTimer_, true);
		timer_.StartCount();
	}
}
