#pragma once
#include "IParticle.h"

/**
* @file MoveParticle.h
* @brief 一定速度で移動するパーティクルのファイル
*/

class MoveParticle :public IParticle
{
private:
	//	速度
	Vector3D spd_;
	//	スケール
	float startScale_ = 1.0f;
	float maxScale_ = 0;
	//	時間
	int32_t timer_ = 0;
	int32_t maxTime_ = 0;
public:
	/**
	* @fn Initialize()
	* 初期化処理関数
	*/
	void Initialize(const Vector3D& pos,
		const Vector3D& velocity,
		float startScale,
		float maxScale,
		int32_t lifeTime,
		int32_t texHandle);
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;
};

