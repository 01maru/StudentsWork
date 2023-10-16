#pragma once
#include "VertexStruct.h"
#include <wrl.h>
#include <d3d12.h>
#include <vector>

/**
* @file VertIdxBuff.h
* @brief 頂点とインデックスバッファをまとめたファイル
*/

class VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

protected:
	//	vertex_
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_;

	//	index
	bool ibActive_ = false;
	D3D12_INDEX_BUFFER_VIEW ibView_{};
	ComPtr<ID3D12Resource> indexBuff_;

	D3D12_RESOURCE_DESC resDesc_{};

private:
	void SetResDesc(UINT size);

protected:
	void Initialize(uint32_t sizeVB, const std::vector<uint16_t>& indices);
	void Initialize(uint32_t sizeVB);
	void IASetVertIdxBuff();

public:
	VertIdxBuff() {};
	virtual ~VertIdxBuff() = default;

	virtual void SetVertices() = 0;
	void SetIndices(const std::vector<uint16_t>& indices);

#pragma region Getter

	const D3D12_RESOURCE_DESC& GetResDesc() { return resDesc_; }

#pragma endregion
};
