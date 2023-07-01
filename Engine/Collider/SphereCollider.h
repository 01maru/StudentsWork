#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "Vector3D.h"

class SphereCollider :public BaseCollider, public Sphere
{
private:
	Vector3D offset_;
public:
	SphereCollider(const Vector3D& offset = Vector3D(), float radius = 1.0f);

	void Update() override;

	//	Getter
	inline float GetRadius() { return radius_; }
	inline const Vector3D& GetOffset() { return offset_; }
	
	//	Setter
	inline void SetOffset(const Vector3D& offset) { offset_ = offset; }
	inline void SetRadius(float radius) { radius_ = radius; }
};

