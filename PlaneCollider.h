#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "Vector3D.h"

class PlaneCollider :public BaseCollider, public Plane
{
public:
	PlaneCollider(const Vector3D& normal);
	void Update() override;
};

