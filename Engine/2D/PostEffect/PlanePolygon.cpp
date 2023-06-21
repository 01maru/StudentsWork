#include "PlanePolygon.h"
#include "DirectX.h"
#include <cassert>

void PlanePolygon::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);

	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	ScreenVertex* vertMap = nullptr;
	HRESULT result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (size_t i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);
}

void PlanePolygon::Initialize()
{
	vertices_.clear();
	vertices_.push_back({ {-1.0f,-1.0f,0.0f},{0,1} });
	vertices_.push_back({ {-1.0f, 1.0f,0.0f},{0,0} });
	vertices_.push_back({ { 1.0f,-1.0f,0.0f},{1,1} });
	vertices_.push_back({ { 1.0f, 1.0f,0.0f},{1,0} });

	uint32_t sizePV = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());
	//	インデックスデータ
	indices_.push_back(0);
	indices_.push_back(1);
	indices_.push_back(2);
	indices_.push_back(2);
	indices_.push_back(1);
	indices_.push_back(3);
	VertIdxBuff::Initialize(sizePV, indices_);
}

void PlanePolygon::DrawIndexedInstanced()
{
	MyDirectX::GetInstance()->GetCmdList()->DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}
