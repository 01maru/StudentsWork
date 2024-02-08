#pragma once
#include "BossState.h"
#include "FrameCounter.h"

/**
* @file BossWayBullets.h
* @brief ボスの5Way弾攻撃の動きを管理するファイル
*/

class BossWayBullets :public BossState
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
	MNE::FrameCounter rate_;
	int32_t bulletMaxNum_ = 5;
	int32_t delayTime_ = 60;
	float bulletSpd_ = 1.0f;
	int32_t bulletLifeTime_ = 600;
	//	単位はradian
	float wayAngle_;

};
