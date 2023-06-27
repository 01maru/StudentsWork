#include "PostEffect.h"
#include "DirectX.h"
#include "TextureManager.h"
#include "PipelineManager.h"
#include <cassert>

#include "ConstBuffStruct.h"

void PostEffect::Initialize(int32_t width, int32_t height, DXGI_FORMAT format)
{
	width_ = width;
	height_ = height;

	texture_.resize(texNum_);
	for (size_t i = 0; i < texNum_; i++)
	{
		texture_[i] = TextureManager::GetInstance()->CreateNoneGraphTexture("postEffect");
	}

#pragma region  ConstBuffer

	material_.Initialize((sizeof(CBuff::CBuffColorMaterial) + 0xFF) & ~0xFF);

	HRESULT result = material_.GetResource()->Map(0, nullptr, (void**)&cMaterialMap_);	//	マッピング
	assert(SUCCEEDED(result));
	SetColor(color_);

#pragma endregion

	PlanePolygon::Initialize();

	//	ビューポート
	viewPortSciRect_.InitializeVP(width, height, 0, 0, 0.0f, 1.0f, texNum_);
	// シザー矩形
	viewPortSciRect_.InitializeSR(0, width, 0, height, texNum_);

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

	for (size_t i = 0; i < texNum_; i++)
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
	heapDesc.NumDescriptors = texNum_;
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
	for (size_t i = 0; i < texNum_; i++)
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
	for (size_t i = 0; i < texNum_; i++)
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

void PostEffect::Initialize(int32_t width, int32_t height, int32_t textureNum, DXGI_FORMAT format)
{
	width_ = width;
	height_ = height;

	texNum_ = textureNum;
	texture_.resize(textureNum);
	for (size_t i = 0; i < textureNum; i++)
	{
		texture_[i] = TextureManager::GetInstance()->CreateNoneGraphTexture("postEffect");
	}

#pragma region  ConstBuffer

	material_.Initialize((sizeof(CBuff::CBuffColorMaterial) + 0xFF) & ~0xFF);

	HRESULT result = material_.GetResource()->Map(0, nullptr, (void**)&cMaterialMap_);	//	マッピング
	assert(SUCCEEDED(result));
	SetColor(color_);

#pragma endregion

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

void PostEffect::Draw(GPipeline* pipeline, bool shadow, int32_t handle1)
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();
	pipeline->SetGraphicsRootSignature();
	pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertIdxBuff::IASetVertIdxBuff();
	
	if (shadow == false) {
		//	テクスチャ
		cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(texture_[0]->GetHandle()));
		if (handle1 != -1) {
			cmdList->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetTextureHandle(handle1));
		}
		else {
			cmdList->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetTextureHandle(texture_[0]->GetHandle()));
		}
		cmdList->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetTextureHandle(texture_[0]->GetHandle()));
		cmdList->SetGraphicsRootDescriptorTable(3, TextureManager::GetInstance()->GetTextureHandle(texture_[0]->GetHandle()));
		material_.SetGraphicsRootCBuffView(4);
	}
	else {
		//	テクスチャ
		cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(texture_[0]->GetHandle()));
		material_.SetGraphicsRootCBuffView(1);
	}

	PlanePolygon::DrawIndexedInstanced();
}

void PostEffect::DrawDoF()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();
	GPipeline* pipeline = PipelineManager::GetInstance()->GetPipeline("dof");
	pipeline->SetGraphicsRootSignature();
	pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertIdxBuff::IASetVertIdxBuff();

	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(texture_[1]->GetHandle()));
	cmdList->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetTextureHandle(texture_[0]->GetHandle()));

	PlanePolygon::DrawIndexedInstanced();
}

void PostEffect::SetColor(const Vector4D& color)
{
	cMaterialMap_->color = color;
}

void PostEffect::Draw()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(texture_[0]->GetHandle()));
	material_.SetGraphicsRootCBuffView(1);

	PlanePolygon::DrawIndexedInstanced();
}

//void PostEffect::SetGlayScale(bool active)
//{
//}

void PostEffect::RSSetVPandSR()
{
	viewPortSciRect_.RSSetVPandSR();
}

void PostEffect::DrawLuminnce()
{
	GPipeline* pipeline = PipelineManager::GetInstance()->GetPipeline("Luminnce", GPipeline::NONE_BLEND);

	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();
	pipeline->SetGraphicsRootSignature();
	pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertIdxBuff::IASetVertIdxBuff();
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(texture_[0]->GetHandle()));
	material_.SetGraphicsRootCBuffView(1);

	PlanePolygon::DrawIndexedInstanced();
}

void PostEffect::SetGPipelineAndIAVertIdxBuff(GPipeline* pipeline)
{
	pipeline->SetGraphicsRootSignature();
	pipeline->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	VertIdxBuff::IASetVertIdxBuff();
}
