#pragma once
#include "BossState.h"
#include "FrameCounter.h"
#include "Vector3D.h"

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

	MNE::FrameCounter rate_;

	MyMath::Vector3D dir_;

	float length_ = 80.0f;
};

