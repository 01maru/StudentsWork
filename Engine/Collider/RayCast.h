#pragma once
#include "Vector3D.h"


namespace MNE
{
	class BaseCollider;
	class Object3D;

	struct RayCast
	{
		Object3D* object = nullptr;
		BaseCollider* collider = nullptr;
		MyMath::Vector3D inter;
		float distance = 0.0f;
	};

}

