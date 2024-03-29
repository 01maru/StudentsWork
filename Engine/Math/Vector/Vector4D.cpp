#include "Vector4D.h"
#include "Vector3D.h"
#include "Matrix.h"
#include <cmath>

MyMath::Vector4D::Vector4D() :x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

MyMath::Vector4D::Vector4D(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {}

MyMath::Vector4D::Vector4D(const Vector3D& v, float w) :x(v.x), y(v.y), z(v.z), w(w) {}

float MyMath::Vector4D::GetLength() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

MyMath::Vector4D& MyMath::Vector4D::GetNormalize()
{
	float len = GetLength();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

void MyMath::Vector4D::Normalize()
{
	float len = GetLength();

	if (len != 0)	*this /= len;
}

void MyMath::Vector4D::SetVec3D(const Vector3D& vec3)
{
	x = vec3.x;
	y = vec3.y;
	z = vec3.z;
}

MyMath::Vector3D MyMath::Vector4D::GetVec3D()
{
	return Vector3D(x, y, z);
}

MyMath::Vector4D MyMath::Vector4D::operator+() const
{
	return *this;
}

MyMath::Vector4D MyMath::Vector4D::operator-() const
{
	return Vector4D(-x, -y, -z, -w);
}

MyMath::Vector4D& MyMath::Vector4D::operator-=(const Vector4D& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

MyMath::Vector4D& MyMath::Vector4D::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

MyMath::Vector4D& MyMath::Vector4D::operator*=(const Matrix& m)
{
	Vector4D ans;
	ans.x = x * m.m[0][0] + y * m.m[1][0] + z * m.m[2][0] + w * m.m[3][0];
	ans.y = x * m.m[0][1] + y * m.m[1][1] + z * m.m[2][1] + w * m.m[3][1];
	ans.z = x * m.m[0][2] + y * m.m[1][2] + z * m.m[2][2] + w * m.m[3][2];
	ans.w = x * m.m[0][3] + y * m.m[1][3] + z * m.m[2][3] + w * m.m[3][3];
	*this = ans;
	return *this;
}

bool MyMath::Vector4D::operator!=(const Vector4D& vec)
{
	return (this->x != vec.x) || (this->y != vec.y) || (this->z != vec.z) || (this->w != vec.w);
}

MyMath::Vector4D MyMath::operator-(const Vector4D& v1, const Vector4D& v2)
{
	Vector4D temp(v1);
	return temp -= v2;
}
