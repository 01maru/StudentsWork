#include "PlayerMoveState.h"
#include "Player.h"

Player* PlayerMoveState::sPlayer_ = nullptr;

void PlayerMoveState::SetPlayer(Player* player)
{
	sPlayer_ = player;
}
