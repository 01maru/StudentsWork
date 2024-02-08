#pragma once
#include "Vector3D.h"
#include "Object3D.h"

struct QueryHit
{
	MNE::Object3D* object = nullptr;
	BaseCollider* collider = nullptr;
	MyMath::Vector3D inter;
	MyMath::Vector3D reject;
};


class QueryCallBack
{
public:
	QueryCallBack() = default;
	virtual ~QueryCallBack() = default;

	virtual bool OnQueryHit(const QueryHit& info) = 0;
};