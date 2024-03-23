#pragma once
#include "BossState.h"
#include "FrameCounter.h"

/**
* @file BossIdleState.h
* @brief ボスの攻撃選択中の動きを管理するファイル
*/

class BossIdleState :public BossState
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
	int32_t idleTime_ = 180;
	MNE::FrameCounter timer_;

private:
	void MoveUpdate();
};

