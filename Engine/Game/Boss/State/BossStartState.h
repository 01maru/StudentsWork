#pragma once
#include "BossState.h"
#include "FrameCounter.h"

/**
* @file BossStartState.h
* @brief ボスの開始時演出を管理するファイル
*/

class BossStartState :public BossState
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
	//	アニメーションタイマー
	FrameCounter timer_;
	int32_t roarTime_ = 240;
};

