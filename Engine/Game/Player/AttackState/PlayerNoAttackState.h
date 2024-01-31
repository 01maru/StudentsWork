#pragma once
#include "PlayerAttackState.h"

class PlayerNoAttackState :public PlayerAttackState
{
public:
	void Update() override;
};

