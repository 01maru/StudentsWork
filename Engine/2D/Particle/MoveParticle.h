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
	//	字間
	int timer_ = 0;
	int maxTime_ = 0;

public:
	void Initialize(const Vector3D& pos,
		const Vector3D& velocity,
		float startScale,
		float maxScale,
		int lifeTime);
	void Update() override;
};

