#pragma once
#include "BossState.h"
#include "FrameCounter.h"

/**
* @file BossBulletState.h
* @brief ボスの弾攻撃の動きを管理するファイル
*/

class BossBulletState :public BossState
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
	int32_t rateTime_ = 60;
	MNE::FrameCounter rate_;
	int32_t bulletMaxNum_ = 5;
	int32_t bulletNum_ = 0;
	float bulletSpd_ = 1.0f;
	int32_t bulletLifeTime_ = 600;

};

