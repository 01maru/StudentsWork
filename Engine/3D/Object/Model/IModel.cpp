#include "IModel.h"

void IModel::Initialize(const char* filename, bool smoothing)
{
	LoadModel(filename, smoothing);

	for (auto& m : meshes_) {
		m.Initialzie();
	}

	for (auto& m : materials_) {
		m.second->Update();
	}

	for (auto& m : materials_) {
		m.second->LoadTexture();
	}
}

void IModel::Draw(int32_t cBuffMtlIdx)
{
	for (auto& mesh : meshes_) {
		mesh.SetGraphicsRootCBuffViewMtl(cBuffMtlIdx);
		mesh.Draw();
	}
}
