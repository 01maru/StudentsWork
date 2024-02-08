#include "Vector2D.h"
#include <cmath>

MyMath::Vector2D MyMath::Vector2D::operator+() const
{
	return *this;
}

MyMath::Vector2D& MyMath::Vector2D::operator+=(const MyMath::Vector2D& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

MyMath::Vector2D& MyMath::Vector2D::operator-=(const MyMath::Vector2D& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

float MyMath::Vector2D::GetLength() const
{
	return sqrt(x * x + y * y);
}

void MyMath::Vector2D::Normalize()
{
	float len = GetLength();

	//	0ベクトルじゃなかったら
	if (len != 0)	*this /= len;
}

MyMath::Vector2D& MyMath::Vector2D::GetNormalize()
{
	float len = GetLength();

	//	0ベクトルじゃなかったら
	if (len != 0)	return *this /= len;

	return *this;
}

MyMath::Vector2D& MyMath::Vector2D::operator/=(float s)
{
	x /= s;
	y /= s;
	return *this;
}

MyMath::Vector2D& MyMath::Vector2D::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

bool MyMath::Vector2D::operator<(const MyMath::Vector2D& v)
{
	return x < v.x && y < v.y;
}

bool MyMath::Vector2D::operator>(const MyMath::Vector2D& v)
{
	return x > v.x && y > v.y;
}

bool MyMath::Vector2D::operator!=(const MyMath::Vector2D& vec)
{
	return (this->x != vec.x) || (this->y != vec.y);
}

bool MyMath::Vector2D::operator==(const MyMath::Vector2D& vec)
{
	return (this->x == vec.x) && (this->y == vec.y);
}

MyMath::Vector2D MyMath::operator-(const MyMath::Vector2D& v1, const MyMath::Vector2D& v2)
{
	MyMath::Vector2D temp(v1);
	return temp -= v2;
}

MyMath::Vector2D MyMath::operator+(const MyMath::Vector2D& v1, const MyMath::Vector2D& v2)
{
	MyMath::Vector2D temp(v1);
	return temp += v2;
}

MyMath::Vector2D MyMath::operator*(const MyMath::Vector2D& v, float s)
{
	MyMath::Vector2D temp(v);
	return temp *= s;
}

MyMath::Vector2D MyMath::operator*(float s, const MyMath::Vector2D& v)
{
	return v * s;
}

MyMath::Vector2D MyMath::operator/(const MyMath::Vector2D& v, float s)
{
	MyMath::Vector2D temp(v);
	return temp /= s;
}

MyMath::Vector2D MyMath::operator/(float s, const MyMath::Vector2D& v)
{
	return v / s;
}

bool MyMath::operator<(const MyMath::Vector2D& v1, const MyMath::Vector2D& v2)
{
	MyMath::Vector2D temp(v1);
	return temp < v2;
}

bool MyMath::operator>(const MyMath::Vector2D& v1, const MyMath::Vector2D& v2)
{
	MyMath::Vector2D temp(v1);
	return temp > v2;
}
