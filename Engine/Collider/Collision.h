#pragma once
#include "CollisionPrimitive.h"

class Collision
{
public:
	static void ClosestPtPoint2Triangle(const MyMath::Vector3D& point, const Triangle& triangle,
		MyMath::Vector3D* closest);
	//	Sphere
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB,
		MyMath::Vector3D* inter = nullptr, MyMath::Vector3D* reject = nullptr);
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane,
		MyMath::Vector3D* inter = nullptr);
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
		MyMath::Vector3D* inter = nullptr, MyMath::Vector3D* reject = nullptr);
	//	Ray
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, MyMath::Vector3D* inter = nullptr);
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere,
		float* distance = nullptr, MyMath::Vector3D* inter = nullptr);
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle,
		float* distance = nullptr, MyMath::Vector3D* inter = nullptr);
};

