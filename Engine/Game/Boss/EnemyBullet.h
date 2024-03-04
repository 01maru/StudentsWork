#pragma once
#include "IBullet.h"

/**
* @file EnemyBullet.h
* @brief ボスの弾を管理するファイル
*/

class EnemyBullet :public IBullet
{
public:
	//	デストラクタ
	~EnemyBullet();
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

	void OnCollision(MNE::CollisionInfo& info) override;
};
