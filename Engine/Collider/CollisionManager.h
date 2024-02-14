#pragma once
#include <forward_list>
#include <d3d12.h>
#include "CollisionPrimitive.h"
#include "BaseCollider.h"
#include <memory>


namespace MNE
{
#pragma region 前置宣言

	struct RayCast;
	class QueryCallBack;

#pragma endregion

	class CollisionManager
	{
	private:
		CollisionManager() {};
		~CollisionManager() = default;
	public:
		static CollisionManager* GetInstance();
		CollisionManager(const CollisionManager&) = delete;
		CollisionManager& operator=(const CollisionManager&) = delete;

		void RemoveCollider();

	private:
		//	コライダーをまとめたリスト
		std::forward_list<std::unique_ptr<BaseCollider>> colliders_;
		std::unique_ptr<Cylinder> stageCollider_;

	public:

		void CheckCollision(BaseCollider* collider, uint16_t attribute);
	
		void AddCollider(std::unique_ptr<BaseCollider> collider) { colliders_.push_front(std::move(collider)); }

		bool Raycast(const Ray& ray, RayCast* hitinfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);
		bool Raycast(const Ray& ray, uint16_t attribute, RayCast* hitinfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

		MyMath::Vector3D CollisionStage(const Sphere& sphere);
		void AddStageCollider(std::unique_ptr<Cylinder>& cylinder) { stageCollider_ = std::move(cylinder); }

		void QuerySphere(const Sphere& sphere, QueryCallBack* callback, uint16_t attribute = (uint16_t)0xffff);
	};

}
