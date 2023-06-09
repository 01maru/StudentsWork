#pragma once
#include "Vector3D.h"

class Object3D;
class BaseCollider;

struct CollisionInfo
{
private:
	Object3D* object = nullptr;
	BaseCollider* collider = nullptr;
	Vector3D inter;

public:
	CollisionInfo(Object3D* object, BaseCollider* collider,
		const Vector3D& inter) {
		this->object = object;
		this->collider = collider;
		this->inter = inter;
	}
};

