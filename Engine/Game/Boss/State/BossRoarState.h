#pragma once
#include "BossState.h"
#include "FrameCounter.h"
#include "Vector3D.h"

/**
* @file BossRoarState.h
* @brief ボスの第二形態移行演出を管理するファイル
*/

class BossRoarState :public BossState
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
	MNE::FrameCounter timer_;
	int32_t roarTime_ = 320;

	//	フォグ
	int32_t fogStartTime_ = 110;
	MNE::FrameCounter fogTimer_;
	int32_t fogTime_ = 40;
	//	パラメーター
	MyMath::Vector3D startColor_ = { 1.0f,1.0f,1.0f };
	MyMath::Vector3D endColor_ = { 0.4f,0.04f,0.04f };
	float fogSStart_ = 2.2f;
	float fogEStart_ = 2.0f;
	float fogSEnd_ = 6.0f;
	float fogEEnd_ = 4.0f;
	float fogSNear_ = 0.1f;
	float fogENear_ = 2.0f;

private:
	/**
	* @fn FogStartCount()
	* フォグの色変更タイマーの開始処理関数
	*/
	void FogStartCount();
	/**
	* @fn FogColorUpdate()
	* フォグの色変更用更新処理関数
	*/
	void FogColorUpdate();
};

