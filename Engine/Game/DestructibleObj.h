#pragma once
#include "Object3D.h"

class DestructibleObj :public MNE::Object3D
{
public:
	void Update();

	void OnCollision(MNE::CollisionInfo& info) override;

private:
	int32_t hp_ = 5;
};

