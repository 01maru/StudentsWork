#pragma once
#include "EnemyState.h"
#include "FrameCounter.h"

class BossIdleState :public EnemyState
{
private:
	enum BossAtState {
		BulletState = 0,
		WayBulletsState,
		StateNum,
	};
public:
	void Initialize() override;
	void Update() override;
private:
	int32_t idleTime_ = 60;
	FrameCounter timer_;
};

