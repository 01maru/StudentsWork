#pragma once
#include "EnemyState.h"
#include "FrameCounter.h"

class BossBulletState :public EnemyState
{
private:
	FrameCounter rate_;
	int32_t bulletMaxNum_;
	int32_t bulletNum_ = 0;

public:
	void Initialize() override;
	void Update() override;
};

