#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

#pragma comment(lib, "d3d12.lib")

class DepthStencil
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> depthBuff_;
	ComPtr<ID3D12DescriptorHeap> dsvHeap_;

	//D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
public:
	void Initialize(int32_t width, int32_t height, DXGI_FORMAT format);
	void OMSetRenderTargets(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle);
	void ClearDepthStencilView(D3D12_CLEAR_FLAGS flags);

	//	Getter
	DXGI_FORMAT GetDSVFormat() { return depthBuff_->GetDesc().Format; }
	//D3D12_DEPTH_STENCIL_DESC GetDSDesc() { return depthStencilDesc; }
	ID3D12DescriptorHeap* GetDSVHeap() { return dsvHeap_.Get(); }

	//	Setter
	//void SetDepthStencilDesc(D3D12_DEPTH_STENCIL_DESC desc) { depthStencilDesc = desc; }
};

