#pragma once
#include "EnemyState.h"
#include "FrameCounter.h"

class BossIdleState :public EnemyState
{
private:
	FrameCounter timer_;
public:
	void Initialize() override;
	void Update() override;
};

