#include "PlayerMoveState.h"
#include "Player.h"

Player* PlayerMoveState::sPlayer_ = nullptr;

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void PlayerMoveState::SetPlayer(Player* player)
{
	sPlayer_ = player;
}
