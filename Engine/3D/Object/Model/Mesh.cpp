#include "Mesh.h"
#include "DirectX.h"
#include "TextureManager.h"
#include "Material.h"
#include <cassert>

using namespace MyMath;

void MNE::Mesh::Initialzie()
{
	uint32_t sizeVB = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());

	VertIdxBuff::Initialize(sizeVB, indices_);
}

void MNE::Mesh::SetGraphicsRootCBuffViewMtl(int32_t index)
{
	mtl_->SetGraphicsRootCBuffView(static_cast<uint32_t>(index));
}

void MNE::Mesh::Draw()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	VertIdxBuff::IASetVertIdxBuff();

	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(mtl_->GetTextureHandle()));

	cmdList->DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void MNE::Mesh::CalcSmoothedNormals()
{
	for (auto itr = smoothData_.begin(); itr != smoothData_.end(); ++itr) {
		std::vector<uint16_t>& v = itr->second;

		Vector3D normal;
		for (uint16_t index : v) {
			normal += vertices_[index].normal;
		}
		normal /= (float)v.size();
		normal.Normalize();

		for (uint16_t index : v) {
			vertices_[index].normal = normal;
		}
	}
}

void MNE::Mesh::SetBone(size_t vertexID, uint16_t boneIndex, float weight)
{
	for (size_t i = 0; i < 4; i++) {
		//	ボーンのweightが0だったら値代入
		if (vertices_[vertexID].boneWeight[i] == 0.0f) {
			vertices_[vertexID].boneIndex[i] = boneIndex;
			vertices_[vertexID].boneWeight[i] = weight;
			return;
		}
	}
}

void MNE::Mesh::SetTextureFilePath(const std::string& filePath)
{
	mtl_->filepath_ = filePath;
}

void MNE::Mesh::SetVertices()
{
	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	ModelVertex* vertMap = nullptr;
	HRESULT result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (size_t i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);
}
