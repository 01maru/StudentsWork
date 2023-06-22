#include "IPostEffect.h"
#include "GPipeline.h"
#include "TextureManager.h"
#include "DirectX.h"

void IPostEffect::Initialize(int32_t width, int32_t height, int32_t textureNum, DXGI_FORMAT format)
{
	HRESULT result;

	texture_.resize(textureNum);
	for (size_t i = 0; i < textureNum; i++)
	{
		texture_[i] = TextureManager::GetInstance()->CreateNoneGraphTexture("postEffect");
	}

	PlanePolygon::Initialize();

	//	ビューポート
	viewPortSciRect_.InitializeVP(width, height, 0, 0, 0.0f, 1.0f, textureNum);
	// シザー矩形
	viewPortSciRect_.InitializeSR(0, width, 0, height, textureNum);

	auto resDesc = MyDirectX::GetInstance()->GetBackBuffDesc();
	resDesc.Format = format;
	resDesc.Width = width;
	resDesc.Height = height;
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	float clsClr[4] = { 1.0f,1.0f,1.0f,1.0f };
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = format;
	clearValue.DepthStencil.Depth = 1.0f;
	for (size_t i = 0; i < 4; i++)
	{
		clearValue.Color[i] = clsClr[i];
	}

	for (size_t i = 0; i < textureNum; i++)
	{
		result = MyDirectX::GetInstance()->GetDev()->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(texture_[i]->GetResourceBuffAddress()));
	}

#pragma region RTV
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = MyDirectX::GetInstance()->GetRTVHeapDesc();
	//	heap
	heapDesc.NumDescriptors = textureNum;
	result = MyDirectX::GetInstance()->GetDev()->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(rtvHeap_.ReleaseAndGetAddressOf()));

	D3D12_RENDER_TARGET_VIEW_DESC _rtvDesc = {};
	_rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	if (format == DXGI_FORMAT_R8G8B8A8_UNORM) {
		_rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	}
	else {
		_rtvDesc.Format = format;
	}

	//	RTV
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_ = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	for (size_t i = 0; i < textureNum; i++)
	{
		rtvHandle_.ptr += MyDirectX::GetInstance()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * i;
		MyDirectX::GetInstance()->GetDev()->CreateRenderTargetView(
			texture_[i]->GetResourceBuff(),
			&_rtvDesc,
			rtvHandle_);
	}
#pragma endregion

#pragma region SRV
	D3D12_SHADER_RESOURCE_VIEW_DESC _srvDesc = {};
	_srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	_srvDesc.Format = _rtvDesc.Format;
	_srvDesc.Texture2D.MipLevels = 1;
	_srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	//	SRV
	for (size_t i = 0; i < textureNum; i++)
	{
		size_t incrementSize = MyDirectX::GetInstance()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = MyDirectX::GetInstance()->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart();
		srvHandle.ptr += incrementSize * texture_[i]->GetHandle();

		MyDirectX::GetInstance()->GetDev()->CreateShaderResourceView(
			texture_[i]->GetResourceBuff(),
			&_srvDesc,
			srvHandle);
	}
#pragma endregion

#pragma region 深度バッファ
	dsv_.Initialize(width, height, DXGI_FORMAT_D32_FLOAT);
#pragma endregion
}

void IPostEffect::RSSetViewPortandScisRect()
{
	viewPortSciRect_.RSSetVPandSR();
}

void IPostEffect::SetGPipelineAndIAVertIdxBuff(GPipeline& pipeline)
{
	pipeline.SetGraphicsRootSignature();
	pipeline.SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	VertIdxBuff::IASetVertIdxBuff();
}
