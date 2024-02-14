#pragma once
#include "IBullet.h"
#include "ParticleEmitter.h"

/**
* @file Bullet.h
* @brief 弾の基盤ファイル
*/

class Bullet :public IBullet
{
public:
	//	デストラクタ
	~Bullet();

	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize() override;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;

	void OnCollision(MNE::CollisionInfo& info) override;

private:
	//	エミッターの生存時間
	int32_t emitterLifeTime_ = 30;
	MNE::ParticleEmitter* emitter_ = nullptr;
};

