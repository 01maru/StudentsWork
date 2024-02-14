#include "CollisionInfo.h"

MNE::BaseCollider* MNE::CollisionInfo::GetCollider()
{
	return collider;
}

MyMath::Vector3D& MNE::CollisionInfo::GetInter()
{
	return inter;
}
