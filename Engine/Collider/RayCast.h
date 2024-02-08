#pragma once
#include "Vector3D.h"

class BaseCollider;
class Object3D;

struct RayCast
{
	MNE::Object3D* object = nullptr;
	BaseCollider* collider = nullptr;
	MyMath::Vector3D inter;
	float distance = 0.0f;
};

