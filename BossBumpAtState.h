#pragma once
#include "BossState.h"
#include "FrameCounter.h"

class BossBumpAtState :public BossState
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
	int32_t objNum_;
	int32_t objMaxNum_;

	int32_t rateTime_ = 60;
	MNE::FrameCounter rate_;

	MyMath::Vector3D dir_;
	int32_t bumpLifeTime_ = 30;

	float height_ = 5.0f;
	float length_ = 80.0f;
	float scale_ = 2.0f;
};

