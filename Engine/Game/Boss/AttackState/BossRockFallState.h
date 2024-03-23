#pragma once
#include "BossState.h"
#include "FrameCounter.h"

class BossRockFallState :public BossState
{
public:
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize() override;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;

private:
	MNE::FrameCounter timer_;
	float bulletSpd_ = 1.0f;
};

