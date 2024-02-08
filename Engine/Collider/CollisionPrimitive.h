#pragma once
#include "Vector3D.h"
#include "Vector2D.h"

struct Sphere {
	MyMath::Vector3D center_;
	float radius_ = 1.0f;
};

struct Cylinder {
	MyMath::Vector2D center_;
	float radius_ = 1.0f;
};

struct Plane {
	MyMath::Vector3D normal_ = { 0.0f,1.0f,0.0f };
	float distance = 0.0f;
};

struct Ray
{
	MyMath::Vector3D start;
	MyMath::Vector3D dir = { 1.0f,0.0f,0.0f };
};

class Triangle
{
public:
	MyMath::Vector3D p0;
	MyMath::Vector3D p1;
	MyMath::Vector3D p2;
	MyMath::Vector3D normal;

	void ComputeNormal();
};