#pragma once
#include "Vector3D.h"

namespace MNE
{
	
	class Object3D;

}
class BaseCollider;

struct CollisionInfo
{
private:
	MNE::Object3D* object = nullptr;
	BaseCollider* collider = nullptr;
	MyMath::Vector3D inter;

public:
	CollisionInfo(MNE::Object3D* object, BaseCollider* collider,
		const MyMath::Vector3D& inter) {
		this->object = object;
		this->collider = collider;
		this->inter = inter;
	}
	BaseCollider* GetCollider() { return collider; }
	MyMath::Vector3D& GetInter() { return inter; }
};

