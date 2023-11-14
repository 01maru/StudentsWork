#pragma once
#include "PlayerMoveState.h"

class PlayerWalkState: public PlayerMoveState
{
public:
	void Update() override;

private:
	void SpdUpdate();
	void SetNextState();
};

