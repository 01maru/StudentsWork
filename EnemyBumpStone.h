#pragma once
#include "IBullet.h"

class EnemyBumpStone :public IBullet
{
public:
	//	デストラクタ
	~EnemyBumpStone();
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

private:
	float height_;

public:
	void SetScale(const MyMath::Vector3D& scale);
};

