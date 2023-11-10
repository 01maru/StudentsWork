#pragma once
#include "PlayerAttackState.h"
#include "FrameCounter.h"

class PlayerNormalBulletState :public PlayerAttackState
{
public:
	void Initialize() override;
	void Update() override;

private:
	FrameCounter rate_;

public:
	void SetBulletRate(int32_t rate);
};

