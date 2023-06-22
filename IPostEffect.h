#pragma once
#include "ViewPortScissorRect.h"
#include "Texture.h"
#include <vector>
#include <cstdint>
#include "PlanePolygon.h"
#include "DepthStencil.h"

class GPipeline;
namespace CBuff {
	struct CBuffColorMaterial;
}

class IPostEffect :public PlanePolygon
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	std::vector<Texture*> texture_;

	D3D12_RESOURCE_BARRIER barrierDesc_{};

	ComPtr<ID3D12DescriptorHeap> rtvHeap_;
	//	ビューポートシザー矩形
	ViewPortScissorRect viewPortSciRect_;

	DepthStencil dsv_;

public:
	void Initialize(int32_t width, int32_t height, int32_t textureNum, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	void RSSetViewPortandScisRect();
	void SetGPipelineAndIAVertIdxBuff(GPipeline& pipeline);
};
