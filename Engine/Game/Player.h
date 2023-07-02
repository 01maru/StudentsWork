#pragma once
#include "Object3D.h"

class Player :public Object3D
{
private:
	Vector2D frontVec;
	float spd;

	bool onGround = true;
	Vector3D fallVec;

	static const float MAX_SPD;
	static const int INVINCIBLE_TIME;

public:
	void PlayerInitialize(IModel* model_);
	void Update();
	void CollisionUpdate();
	void OnCollision(const CollisionInfo& info) override;
	float angle() { return mat_.angle_.y; }
};

