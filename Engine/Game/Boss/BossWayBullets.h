#pragma once
#include "EnemyState.h"
#include "FrameCounter.h"

class BossWayBullets :public EnemyState
{
private:
	FrameCounter rate_;
	int32_t bulletMaxNum_ = 5;
	int32_t delayTime_ = 60;
	float bulletSpd_ = 1.0f;
	int32_t bulletLifeTime_ = 600;
	//	ラディウスで
	float wayAngle_;

public:
	void Initialize() override;
	void Update() override;
};
