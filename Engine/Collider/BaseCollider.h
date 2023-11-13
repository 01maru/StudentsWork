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

	bool deleteFlag_ = false;

	uint16_t attribute = 0b1111111111111111;
public:
	friend class CollisionManager;

	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	virtual void Update() = 0;
	inline void OnCollision(CollisionInfo& info) { object_->OnCollision(info); }
	void Finalize() { deleteFlag_ = true; }

	//	Getter
	inline Object3D* GetObject3D() { return object_; }
	inline CollisionShapeType GetShapeType() { return shapeType_; }
	bool GetDeleteFlag() { return deleteFlag_; }
	uint16_t GetAttribute() { return attribute; }

	//	Setter
	inline void SetObject3D(Object3D* object) { object_ = object; }
	inline void SetAttribute(unsigned short attribute_) { attribute = attribute_; }
	inline void AddAttribute(unsigned short attribute_) { attribute |= attribute_; }
	inline void RemoveAttribute(unsigned short attribute_) { attribute &= !attribute_; }
};

