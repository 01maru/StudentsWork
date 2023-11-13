#pragma once
#include "EnemyState.h"
#include "FrameCounter.h"

class BossDeathState :public EnemyState
{
public:
	void Initialize() override;
	void Update() override;
	
private:
	FrameCounter counter_;
};

