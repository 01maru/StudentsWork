#include "PlaneCollider.h"

MNE::PlaneCollider::PlaneCollider(const MyMath::Vector3D& normal)
{
	normal_ = normal;
	shapeType_ = COLLISIONSHAPE_PLANE;
}

void MNE::PlaneCollider::Update()
{
}
