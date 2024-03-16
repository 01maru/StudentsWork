#include "IPostEffect.h"
#include "TextureManager.h"
#include "ConstBuffStruct.h"
#include "DirectX.h"
#include "GPipeline.h"
#include <cassert>

using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void MNE::IPostEffect::Initialize(int32_t width, int32_t height, const std::string& name, int32_t textureNum, DXGI_FORMAT format)
{
	width_ = width;
	height_ = height;

	//	テクスチャ生成
	TextureManager* texMan = TextureManager::GetInstance();
	texture_.resize(textureNum);
	
	for (int32_t i = 0; i < texture_.size(); i++)
	{
		texture_[i] = texMan->CreateNoneGraphTexture(name + "PostEffect" + std::to_string(i));
	}

#pragma region  ConstBuffer
	material_.Initialize((sizeof(CBuff::CBuffColorMaterial) + 0xFF) & ~0xFF);

	HRESULT result = material_.GetResource()->Map(0, nullptr, (void**)&cMaterialMap_);	//	マッピング
	assert(SUCCEEDED(result));
	SetColor(color_);
#pragma endregion
	
	//	ぺらポリゴン初期化
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

	//	深度バッファ
	dsv_.Initialize(width, height, DXGI_FORMAT_D32_FLOAT);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void MNE::IPostEffect::Update()
{
	MyDirectX* dx = MyDirectX::GetInstance();

	dx->PrevPostEffect(this);

	if (originalPE_ != nullptr)
	{
		originalPE_->Draw(mode_);
	}

	dx->PostEffectDraw(this);
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void MNE::IPostEffect::Draw(int32_t /*mode*/)
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	//	テクスチャ
	int32_t rootParaIdx = 0;
	TextureManager* texMan = TextureManager::GetInstance();
	cmdList->SetGraphicsRootDescriptorTable(rootParaIdx++, texMan->GetTextureHandle(texture_[0]->GetHandle()));

	material_.SetGraphicsRootCBuffView(rootParaIdx++);

	PlanePolygon::DrawIndexedInstanced();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

int32_t MNE::IPostEffect::GetWidth() const
{
	return width_;
}

int32_t MNE::IPostEffect::GetHeight() const
{
	return height_;
}

const MyMath::Vector4D& MNE::IPostEffect::GetClearColor()
{
	return clearColor_;
}

int32_t MNE::IPostEffect::GetTextureNum()
{
	return static_cast<int32_t>(texture_.size());
}

Texture* MNE::IPostEffect::GetTexture(int32_t index)
{
	return texture_[index];
}

std::string MNE::IPostEffect::GetName()
{
	return name_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::IPostEffect::RSSetVPandSR()
{
	viewPortSciRect_.RSSetVPandSR();
}

void MNE::IPostEffect::SetGPipelineAndIAVertIdxBuff()
{
	pipeline_->SetGraphicsRootSignature();
	pipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	VertIdxBuff::IASetVertIdxBuff();
}

void MNE::IPostEffect::SetMode(int32_t mode)
{
	mode_ = mode;
}

void MNE::IPostEffect::SetGPipeline(GPipeline* pipeline)
{
	pipeline_ = pipeline;
}

void MNE::IPostEffect::SetColor(const MyMath::Vector4D& color)
{
	cMaterialMap_->color = color;
}

void MNE::IPostEffect::SetClearColor(const MyMath::Vector4D& color)
{
	clearColor_ = color;
}

void MNE::IPostEffect::SetOriginalPostEffect(IPostEffect* original)
{
	originalPE_ = original;
}

//void MNE::IPostEffect::SetOriginal(Original original)
//{
//	original_ = original;
//}
