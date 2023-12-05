#pragma once
#include "Object3D.h"
#include "FrameCounter.h"

class EnemyBullet :public Object3D
{
public:
	~EnemyBullet();
	void Initialize();
	void Update();

private:
	const int32_t LIFE_TIME = 600;
	Vector3D moveVec_;
	float spd_;
	FrameCounter lifeTime_;
	float radius_ = 0.4f;
	int32_t damage_ = 5;
public:
	void SetMoveVec(const Vector3D& moveVec);
	void SetSpd(float spd);
	void SetLifeTime(int32_t time);
	bool GetIsActive();
	void OnCollision(CollisionInfo& info) override;
};
