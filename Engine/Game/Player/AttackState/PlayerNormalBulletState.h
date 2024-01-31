#pragma once
#include "PlayerAttackState.h"
#include "FrameCounter.h"

class PlayerNormalBulletState :public PlayerAttackState
{
public:
	void Initialize() override;
	void Update() override;
private:
	int32_t bulletLifeTime_ = 600;
	float bulletSpd_ = 1.0f;
};

