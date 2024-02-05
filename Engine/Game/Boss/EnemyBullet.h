#pragma once
#include "Object3D.h"
#include "FrameCounter.h"

class EnemyBullet :public MNE::Object3D
{
public:
	//	デストラクタ
	~EnemyBullet() {};
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize();
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();

private:
	const int32_t LIFE_TIME = 600;
	Vector3D moveVec_;
	float spd_;
	FrameCounter lifeTime_;
	float radius_ = 0.4f;
	int32_t damage_ = 5;

public:
	void OnCollision(CollisionInfo& info) override;

#pragma region Getter

	bool GetIsActive();
	
#pragma endregion

#pragma region Setter

	void SetMoveVec(const Vector3D& moveVec);
	void SetSpd(float spd);
	void SetLifeTime(int32_t time);

#pragma endregion
};
