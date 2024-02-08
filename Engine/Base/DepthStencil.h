#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

/**
* @file DepthStencil.h
* @brief 深度ステンシルバッファをまとめたファイル
*/

namespace MNE
{

	class DepthStencil
	{
	private:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		ComPtr<ID3D12Resource> depthBuff_;
		ComPtr<ID3D12DescriptorHeap> dsvHeap_;

		//D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
	public:
		/**
		* @fn Initialize(int32_t, int32_t, DXGI_FORMAT)
		* 初期化用関数
		*/
		void Initialize(int32_t width, int32_t height, DXGI_FORMAT format);
		void OMSetRenderTargets(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle);
		void ClearDepthStencilView(D3D12_CLEAR_FLAGS flags);

#pragma region Getter

		DXGI_FORMAT GetDSVFormat() { return depthBuff_->GetDesc().Format; }
		//D3D12_DEPTH_STENCIL_DESC GetDSDesc() { return depthStencilDesc; }
		ID3D12DescriptorHeap* GetDSVHeap() { return dsvHeap_.Get(); }

#pragma endregion

		//	Setter
		//void SetDepthStencilDesc(D3D12_DEPTH_STENCIL_DESC desc) { depthStencilDesc = desc; }
	};

}
