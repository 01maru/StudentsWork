#include "CollisionPrimitive.h"

using namespace MyMath;

void Triangle::ComputeNormal()
{
	Vector3D p0_p1 = p1 - p0;
	Vector3D p0_p2 = p2 - p0;

	normal = p0_p1.cross(p0_p2);
	normal.Normalize();
}
