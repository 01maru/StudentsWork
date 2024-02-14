#include "SphereCollider.h"
#include "Object3D.h"

using namespace MyMath;

MNE::SphereCollider::SphereCollider(const Vector3D& offset, float radius) :offset_(offset)
{
	radius_ = radius;
	shapeType_ = COLLISIONSHAPE_SPHERE;
}

void MNE::SphereCollider::Update()
{
	const MyMath::Matrix matWorld = GetWorldMatrix();

	Sphere::center_.x = matWorld.m[3][0] + offset_.x;
	Sphere::center_.y = matWorld.m[3][1] + offset_.y;
	Sphere::center_.z = matWorld.m[3][2] + offset_.z;
}
