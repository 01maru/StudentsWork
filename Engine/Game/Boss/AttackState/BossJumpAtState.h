#pragma once
#include "BossState.h"
#include "FrameCounter.h"
#include "Vector3D.h"

/**
* @file BossJumpAtState.h
* @brief ボスのジャンプ攻撃の動きを管理するファイル
*/

class BossJumpAtState :public BossState
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
	//	アニメーション
	FrameCounter timer_;
	FrameCounter animeTimer_;
	int32_t animeTime_ = 180;
	int32_t jumpStartTime_ = 30;
	int32_t jumpingTime_ = 30;

	bool jumping_ = false;
	Vector3D startPos_;
	Vector3D endPos_;

private:
	/**
	* @fn BefJumpUpdate()
	* ジャンプ前の更新処理関数
	*/
	void BefJumpUpdate();
	/**
	* @fn JumpUpdate()
	* ジャンプ中と次のステートへの移行の更新処理関数
	*/
	void JumpUpdate();
};

