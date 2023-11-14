#pragma once
#include "EnemyState.h"

class BossIdleState :public EnemyState
{
public:
	void Update() override;
};

