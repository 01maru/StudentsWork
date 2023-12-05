#pragma once
#include "EnemyState.h"
#include "FrameCounter.h"

class BossWayBullets :public EnemyState
{
private:
	FrameCounter rate_;
	int32_t bulletMaxNum_ = 5;

public:
	void Initialize() override;
	void Update() override;
};
