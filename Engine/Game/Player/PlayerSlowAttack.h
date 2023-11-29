#pragma once
#include "PlayerAttackState.h"
#include <cstdint>

class PlayerSlowAttack :public PlayerAttackState
{
private:
	int32_t rate_ = 5;
	int32_t bulletNum_ = 0;
	int32_t bulletMaxNum_ = 5;
	float spd_ = 1.0f;

public:
	void Initialize() override;
	void Update() override;
};

