#pragma once
#include "Vector3D.h"

namespace MNE
{
#pragma region 前置宣言

	class Object3D;
	class BaseCollider;

#pragma endregion

	struct CollisionInfo
	{
	public:
		//	コンストラクタ
		CollisionInfo(MNE::Object3D* object, BaseCollider* collider, const MyMath::Vector3D& inter);

	private:
		MNE::Object3D* object_ = nullptr;
		BaseCollider* collider_ = nullptr;
		MyMath::Vector3D inter_;

	public:
#pragma region Getter

		BaseCollider* GetCollider();
		MyMath::Vector3D& GetInter();

#pragma endregion
	};

}

