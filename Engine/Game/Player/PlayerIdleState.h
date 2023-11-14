#pragma once
#include "PlayerMoveState.h"

class PlayerIdleState :public PlayerMoveState
{
public:
	void Update() override;
};

