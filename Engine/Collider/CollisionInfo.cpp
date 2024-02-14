#include "CollisionInfo.h"

BaseCollider* CollisionInfo::GetCollider()
{
	return collider;
}

MyMath::Vector3D& CollisionInfo::GetInter()
{
	return inter;
}
