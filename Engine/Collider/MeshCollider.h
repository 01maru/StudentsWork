#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include <vector>
#include "Matrix.h"

class IModel;

class MeshCollider :public BaseCollider
{
private:
	std::vector<Triangle> triangles_;
	Matrix invMatWorld_;

public:
	MeshCollider() { shapeType_ = COLLISIONSHAPE_MESH; }
	void ConstructTriangles(IModel* model);
	void Update() override;

	//	CheckCollision
	bool CheckCollisionSphere(const Sphere& sphere, Vector3D* inter = nullptr, Vector3D* reject = nullptr);
	bool CheckCollisionRay(const Ray& ray, float* dis = nullptr, Vector3D* inter = nullptr);
};

