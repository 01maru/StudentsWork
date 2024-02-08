#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include <vector>
#include "Matrix.h"

namespace MNE
{

	class IModel;
	
}

class MeshCollider :public BaseCollider
{
private:
	std::vector<Triangle> triangles_;
	MyMath::Matrix invMatWorld_;

public:
	MeshCollider() { shapeType_ = COLLISIONSHAPE_MESH; }
	void ConstructTriangles(MNE::IModel* model);
	void Update() override;

	//	CheckCollision
	bool CheckCollisionSphere(const Sphere& sphere, MyMath::Vector3D* inter = nullptr, MyMath::Vector3D* reject = nullptr);
	bool CheckCollisionRay(const Ray& ray, float* dis = nullptr, MyMath::Vector3D* inter = nullptr);
};

