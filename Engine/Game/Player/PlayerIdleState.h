#pragma once
#include "PlayerMoveState.h"

class PlayerIdleState :public PlayerMoveState
{
public:
	void Initialize() override;
	void Update() override;
};

