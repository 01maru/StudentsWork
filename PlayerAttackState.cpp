#include "PlayerAttackState.h"
#include "Player.h"

Player* PlayerAttackState::sPlayer_ = nullptr;

void PlayerAttackState::SetPlayer(Player* player)
{
	sPlayer_ = player;
}
