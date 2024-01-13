#include "CircleShadow.h"
#include <cmath>

void MNE::CircleShadow::SetDir(const Vector3D& dir)
{
	dir_ = dir;
	dir_.Normalize();
}

void MNE::CircleShadow::SetFactorAngle(const Vector2D& factorAngle)
{
	factorAngleCos_.x = cosf(MyMath::ConvertToRad(factorAngle.x));
	factorAngleCos_.y = cosf(MyMath::ConvertToRad(factorAngle.y));
}
