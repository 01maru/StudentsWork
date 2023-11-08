#pragma once
#include "PlayerMoveState.h"
#include <cstdint>

class PlayerAvoidState: public PlayerMoveState
{
public:
	void Initialize() override;
	void Update() override;

private:
	void SpdUpdate();
	void SetNextState();

private:
	int32_t avoidTimer_ = 0;
	int32_t avoidTime_ = 0;
};

