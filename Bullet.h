#pragma once
#include "Object3D.h"
#include "FrameCounter.h"

class Bullet :public Object3D
{
public:
	void Initialize();
	void Update();

private:
	Vector3D moveVec_;
	float spd_;
	FrameCounter lifeTime_;

public:
	void SetMoveVec(const Vector3D& moveVec);
	void SetSpd(float spd);
	void SetLifeTime(int32_t time);
	bool GetIsActive();
};

