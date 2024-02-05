#pragma once
#include "BossState.h"
#include "FrameCounter.h"
#include "Vector3D.h"

/**
* @file BossDeathState.h
* @brief ボスの死亡時演出を管理するファイル
*/

#pragma region 前置宣言
namespace MNE
{
	class ParticleEmitter;
}
#pragma endregion

class BossDeathState :public BossState
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
	int32_t animationTime_ = 270;

	//	フォグ
	int32_t fogStartTime_ = 200;
	FrameCounter fogTimer_;
	int32_t fogTime_ = 60;
	//	パラメーター
	Vector3D startColor_ = { 0.4f,0.04f,0.04f };
	Vector3D endColor_ = { 1.0f,1.0f,1.0f };
	float fogSStart_ = 2.0f;
	float fogEStart_ = 2.2f;
	float fogSEnd_ = 4.0f;
	float fogEEnd_ = 6.0f;
	float fogSNear_ = 2.0f;
	float fogENear_ = 0.1f;

	//	パーティクルエミッター
	MNE::ParticleEmitter* emitter_ = nullptr;

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

