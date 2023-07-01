#pragma once
#include "CollisionType.h"
#include "CollisionInfo.h"
#include "Object3D.h"
#include <cstdint>

class BaseCollider
{
protected:
	Object3D* object_ = nullptr;
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

	uint16_t attribute = 0b1111111111111111;
public:
	friend class CollisionManager;

	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	virtual void Update() = 0;
	inline void OnCollision(const CollisionInfo& info) { object_->OnCollision(info); }

	//	Getter
	inline Object3D* GetObject3D() { return object_; }
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	//	Setter
	inline void SetObject3D(Object3D* object) { object_ = object; }
	inline void SetAttribute(unsigned short attribute_) { attribute = attribute_; }
	inline void AddAttribute(unsigned short attribute_) { attribute |= attribute_; }
	inline void RemoveAttribute(unsigned short attribute_) { attribute &= !attribute_; }
};

