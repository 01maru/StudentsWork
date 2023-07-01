#include "MeshCollider.h"
#include "Collision.h"
#include "IModel.h"
#include "Object3D.h"

void MeshCollider::ConstructTriangles(IModel* model)
{
	triangles_.clear();
	std::vector<Mesh> meshes = model->GetMeshes();
	int start = 0;
	std::vector<Mesh>::const_iterator itr = meshes.cbegin();

	for (; itr != meshes.cend(); ++itr) {
		Mesh mesh = *itr;
		const std::vector<ModelVertex>& vertices = mesh.GetVertices();
		const std::vector<unsigned short>& indices = mesh.GetIndices();

		size_t triangleNum = indices.size() / 3;
		triangles_.resize(triangles_.size() + triangleNum);

		for (int i = 0; i < triangleNum; i++) {
			Triangle& tri = triangles_[start + i];
			int idx0 = indices[i * 3 + 0];
			int idx1 = indices[i * 3 + 1];
			int idx2 = indices[i * 3 + 2];

			tri.p0 = {
				vertices[idx0].pos.x,
				vertices[idx0].pos.y,
				vertices[idx0].pos.z};

			tri.p1 = {
				vertices[idx1].pos.x,
				vertices[idx1].pos.y,
				vertices[idx1].pos.z};

			tri.p2 = {
				vertices[idx2].pos.x,
				vertices[idx2].pos.y,
				vertices[idx2].pos.z};

			tri.ComputeNormal();
		}

		start += (int)triangleNum;
	}
}

void MeshCollider::Update()
{
	InverseMatrix(GetObject3D()->GetMatWorld(), invMatWorld_);
}

bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, Vector3D* inter, Vector3D* reject)
{
	Sphere localSphere;
	localSphere.center_ = Vec3Transform(sphere.center_, invMatWorld_);
	Vector3D vec(invMatWorld_.m[0][0], invMatWorld_.m[0][1], invMatWorld_.m[0][2]);
	localSphere.radius_ *= vec.GetLength();

	std::vector<Triangle>::const_iterator it = triangles_.cbegin();

	for (; it != triangles_.cend(); ++it) {
		const Triangle& triangle = *it;

		if (Collision::CheckSphere2Triangle(localSphere, triangle, inter, reject)) {
			if (inter) {
				const Matrix& matWorld = GetObject3D()->GetMatWorld();

				*inter = Vec3Transform(*inter, matWorld);
			}

			if (reject) {
				const Matrix& matWorld = GetObject3D()->GetMatWorld();
				*reject = Vec3TransformNormal(*reject, matWorld);
			}
			return true;
		}
	}

	return false;
}

bool MeshCollider::CheckCollisionRay(const Ray& ray, float* dis, Vector3D* inter)
{
	// オブジェクトのローカル座標系でのレイを得る
	Ray localRay;
	localRay.start = Vec3Transform(ray.start, invMatWorld_);
	localRay.dir = Vec3TransformNormal(ray.dir, invMatWorld_);

	std::vector<Triangle>::const_iterator it = triangles_.cbegin();

	for (; it != triangles_.cend(); ++it) {
		const Triangle& triangle = *it;
		Vector3D tempInter;

		if (Collision::CheckRay2Triangle(localRay, triangle, nullptr, &tempInter)) {

			const Matrix& matWorld = GetObject3D()->GetMatWorld();
			tempInter = Vec3Transform(tempInter, matWorld);

			if (dis) {
				Vector3D sub = tempInter - ray.start;
				*dis = sub.dot(ray.dir);
			}
			if (inter) {
				*inter = tempInter;
			}
			return true;
		}
	}

	return false;
}
