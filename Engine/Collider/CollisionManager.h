#pragma once
#include <forward_list>
#include <d3d12.h>
#include "CollisionPrimitive.h"
#include "BaseCollider.h"
#include <memory>

struct RayCast;
class QueryCallBack;

class CollisionManager
{
private:
	std::forward_list<std::unique_ptr<BaseCollider>> colliders;

	CollisionManager() {};
	~CollisionManager() = default;
public:
	static CollisionManager* GetInstance();
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;
	
	void CheckAllCollisions();
	void AddCollider(std::unique_ptr<BaseCollider> collider) { colliders.push_front(std::move(collider)); }
	void RemoveCollider() { colliders.remove_if([](std::unique_ptr<BaseCollider>& collider) {
		return collider->GetDeleteFlag();
		});
	}

	bool Raycast(const Ray& ray, RayCast* hitinfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);
	bool Raycast(const Ray& ray, unsigned short attribute, RayCast* hitinfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

	void QuerySphere(const Sphere& sphere, QueryCallBack* callback, unsigned short attribute = (unsigned short)0xffff);
};

