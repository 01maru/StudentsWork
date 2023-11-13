#pragma once
#include "PlayerAttackState.h"
#include "FrameCounter.h"

class PlayerNormalBulletState :public PlayerAttackState
{
public:
	void Initialize() override;
	void Update() override;
};

