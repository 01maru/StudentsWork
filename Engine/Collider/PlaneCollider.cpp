#include "PlaneCollider.h"

PlaneCollider::PlaneCollider(const Vector3D& normal)
{
	normal_ = normal;
	shapeType_ = COLLISIONSHAPE_PLANE;
}

void PlaneCollider::Update()
{
}
