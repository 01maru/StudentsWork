#pragma once
#include "Object3D.h"
#include "SphereCollider.h"

class SuctionEmptyObj :public MNE::Object3D
{
public:
	~SuctionEmptyObj();
	void Initialize();
	void Update();
private:
	std::unique_ptr<MNE::SphereCollider> sphereCol_;

	float suctionV_ = 0.1f;

public:
	void OnCollision(MNE::CollisionInfo& info) override;

	float GetRadius();

	void SetRadius(float rad);
	void SetSuctionValue(float v);
};

