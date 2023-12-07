#pragma once
#include "EnemyState.h"
#include "FrameCounter.h"

class ParticleEmitter;

class BossDeathState :public EnemyState
{
public:
	void Initialize() override;
	void Update() override;
	
private:
	FrameCounter counter_;
	ParticleEmitter* emitter_ = nullptr;
	bool bossHid_ = false;
};

