#include "CollisionInfo.h"

MNE::CollisionInfo::CollisionInfo(MNE::Object3D* object, BaseCollider* collider, const MyMath::Vector3D& inter)
	:object_(object), collider_(collider), inter_(inter)
{
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

MNE::BaseCollider* MNE::CollisionInfo::GetCollider()
{
	return collider_;
}

MyMath::Vector3D& MNE::CollisionInfo::GetInter()
{
	return inter_;
}
