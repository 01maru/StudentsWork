#include "PlayerAttackState.h"
#include "Player.h"
#include "InputManager.h"
#include "PlayerNormalBulletState.h"
#include "PlayerSlowAttack.h"
#include "PlayerNoAttackState.h"

using namespace MNE;

Player* PlayerAttackState::sPlayer_ = nullptr;

std::unique_ptr<PlayerAttackState> PlayerAttackState::SetNextState(int32_t nowState)
{
	InputManager* inputMan = InputManager::GetInstance();

	bool dikKey = false;
	//	スロー攻撃
	bool input = inputMan->GetMouse()->GetClick(InputMouse::RightClick) ||
		inputMan->GetPad()->GetLTrigger() || inputMan->GetPad()->GetRTrigger();
	if (input && sPlayer_->GetSlowAtIsActive())
	{
		if (nowState != SlowAttack)
		{
			sPlayer_->StartSlowAtCT();
			return std::make_unique<PlayerSlowAttack>();
		}
		else 
		{
			dikKey = true;
		}
	}
	
	//	通常攻撃
	input = inputMan->GetMouse()->GetClick(InputMouse::LeftClick) ||
		inputMan->GetPad()->GetLTrigger() || inputMan->GetPad()->GetRTrigger();
	if (input) {
		if (nowState != NormalAttack)
		{
			return std::make_unique<PlayerNormalBulletState>();
		}
		else
		{
			dikKey = true;
		}
	}
	
	if (dikKey == false && nowState != NoAttack)
	{
		return std::make_unique<PlayerNoAttackState>();
	}

	return nullptr;
}

void PlayerAttackState::SetPlayer(Player* player)
{
	sPlayer_ = player;
}
