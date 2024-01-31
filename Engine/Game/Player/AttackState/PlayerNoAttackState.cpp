#include "PlayerNoAttackState.h"
#include "Player.h"

void PlayerNoAttackState::Update()
{
	std::unique_ptr<PlayerAttackState> next_ = SetNextState(NoAttack);

	if (next_ != nullptr) 
	{
		sPlayer_->SetAttackState(next_);
	}		
}
