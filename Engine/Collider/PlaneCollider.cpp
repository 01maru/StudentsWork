#include "PlaneCollider.h"

PlaneCollider::PlaneCollider(const MyMath::Vector3D& normal)
{
	normal_ = normal;
	shapeType_ = COLLISIONSHAPE_PLANE;
}

void PlaneCollider::Update()
{
}
