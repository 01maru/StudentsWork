#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "Vector3D.h"

class SphereCollider :public BaseCollider, public Sphere
{
private:
	MyMath::Vector3D offset_;
public:
	SphereCollider(const MyMath::Vector3D& offset = MyMath::Vector3D(), float radius = 1.0f);

	void Update() override;

	//	Getter
	inline float GetRadius() { return radius_; }
	inline const MyMath::Vector3D& GetOffset() { return offset_; }
	
	//	Setter
	inline void SetOffset(const MyMath::Vector3D& offset) { offset_ = offset; }
	inline void SetRadius(float radius) { radius_ = radius; }
};

