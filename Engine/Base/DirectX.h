#pragma once
#define NOMINMAX
#include "ViewPortScissorRect.h"
#include "DepthStencil.h"
#include "Vector4D.h"

#include <d3d12.h>
#include <dxgi1_6.h>

#include <list>
#include <vector>
#include <wrl.h>

namespace MNE
{
	class PostEffect;

	class MyDirectX
	{
	private:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		ComPtr<ID3D12Device> device_;

		ComPtr<ID3D12CommandAllocator> cmdAllocator_;
		ComPtr<ID3D12GraphicsCommandList> cmdList_;

		ComPtr<ID3D12CommandQueue> cmdQueue_;

		ComPtr<IDXGISwapChain4> swapChain_;

		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
		ComPtr<ID3D12DescriptorHeap> rtvHeap_;

		// バックバッファ
		std::vector<ComPtr<ID3D12Resource>> backBuffers_;
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_;

		ComPtr<ID3D12Fence> fence_;
		UINT64 fenceVal_ = 0;

		D3D12_RESOURCE_BARRIER barrierDesc_{};

		DepthStencil dsv_;

		//	一つだけ
		ComPtr<ID3D12DescriptorHeap> srvHeap_;

		//	ビューポートシザー矩形
		ViewPortScissorRect viewPortSciRect_;

	private:
		void DebugLayer();

		void ScreenClear(const MyMath::Vector4D& clearColor, D3D12_CPU_DESCRIPTOR_HANDLE& rtvHandle);

		void SetResourceBarrier(D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, ID3D12Resource* pResource = nullptr);
		void CmdListDrawAble(ID3D12Resource* pResource, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter,
			D3D12_CPU_DESCRIPTOR_HANDLE& rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE& dsvHandle, int32_t rtDescNum = 1, const MyMath::Vector4D& clearColor = MyMath::Vector4D(0.1f, 0.25f, 0.5f, 0.0f));
		void CmdListCloseAndFlip();

		MyDirectX() {};
		~MyDirectX() {};
	public:
		static MyDirectX* GetInstance();
		MyDirectX(const MyDirectX& obj) = delete;
		MyDirectX& operator=(const MyDirectX& obj) = delete;

		void Initialize();

		void PrevPostEffect(MNE::PostEffect* postEffect, const MyMath::Vector4D& clearColor = MyMath::Vector4D(0.1f, 0.25f, 0.5f, 0.0f));
		void PostEffectDraw(MNE::PostEffect* postEffect);

		void PrevDraw(const MyMath::Vector4D& clearColor = MyMath::Vector4D(0.1f, 0.25f, 0.5f, 0.0f));
		void PostDraw();

		void DrawEnd();

		//	Getter
		const D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSRVHeapForHeapStart() { return srvHeap_->GetCPUDescriptorHandleForHeapStart(); }
		const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSRVHeapForHeapStart() { return srvHeap_->GetGPUDescriptorHandleForHeapStart(); }

		ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap_.Get(); }
		ID3D12Device* GetDev() { return device_.Get(); }
		ID3D12GraphicsCommandList* GetCmdList() { return cmdList_.Get(); }
		D3D12_RESOURCE_DESC GetBackBuffDesc() { return backBuffers_[0]->GetDesc(); }
		D3D12_DESCRIPTOR_HEAP_DESC GetRTVHeapDesc() { return rtvHeap_->GetDesc(); }
	};

}
