#pragma once
#include "IBullet.h"

class RockObjBullet :public IBullet
{
public:
	//	デストラクタ
	~RockObjBullet();
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
	bool onGround_ = FALSE;
	MyMath::Vector3D acc_;

	void OnGroundUpdate();
};

