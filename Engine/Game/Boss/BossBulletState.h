#pragma once
#include "BossState.h"
#include "FrameCounter.h"

class BossBulletState :public BossState
{
private:
	int32_t rateTime_ = 60;
	FrameCounter rate_;
	int32_t bulletMaxNum_ = 5;
	int32_t bulletNum_ = 0;
	float bulletSpd_ = 1.0f;
	int32_t bulletLifeTime_ = 600;

public:
	void Initialize() override;
	void Update() override;
};

