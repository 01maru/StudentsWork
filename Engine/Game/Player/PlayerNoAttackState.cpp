#include "PlayerNoAttackState.h"
#include "InputManager.h"
#include "Player.h"
#include "PlayerNormalBulletState.h"

void PlayerNoAttackState::Update()
{
	InputManager* input = InputManager::GetInstance();
	if (input->GetMouse()->GetClick(InputMouse::LeftClick) ||
		input->GetPad()->GetLTrigger() || input->GetPad()->GetRTrigger()) {
		std::unique_ptr<PlayerAttackState> next_ = std::make_unique<PlayerNormalBulletState>();
		sPlayer_->SetAttackState(next_);
	}

		
}
