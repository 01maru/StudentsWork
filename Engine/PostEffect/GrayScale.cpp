#include "GrayScale.h"
#include "ConstBuffStruct.h"
#include "DirectX.h"
#include "TextureManager.h"
#include "PipelineManager.h"
#include "Shader.h"
#include <cassert>

using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void MNE::GrayScale::Initialize(int32_t width, int32_t height, const std::string& name, int32_t textureNum, DXGI_FORMAT format)
{
#pragma region  ConstBuffer

	activeGray_.Initialize((sizeof(CBuff::CBuffGrayScale) + 0xFF) & ~0xFF);
	HRESULT result = activeGray_.GetResource()->Map(0, nullptr, (void**)&cGrayScaleMap_);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region Pipeline

	PipelineManager* pipeMan = PipelineManager::GetInstance();
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	//	xyz座標
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }		//	uv座標
	};

	Shader shader("ScreenVS", "GrayScalePS");

	std::unique_ptr<GPipeline> pipeline = std::make_unique<GPipeline>();
	pipeline->Initialize(shader, inputLayout, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO,
		true, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);

	pipeline_ = pipeMan->AddPipeline(pipeline, "GrayScale");

#pragma endregion

	IPostEffect::Initialize(width, height, name, textureNum, format);
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void MNE::GrayScale::Draw(int32_t /*mode*/)
{
	SetGPipelineAndIAVertIdxBuff();

	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	int32_t rootParaIdx = 0;
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(rootParaIdx++, TextureManager::GetInstance()->GetTextureHandle(GetTexture()->GetHandle()));

	material_.SetGraphicsRootCBuffView(rootParaIdx++);
	activeGray_.SetGraphicsRootCBuffView(rootParaIdx++);

	PlanePolygon::DrawIndexedInstanced();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool MNE::GrayScale::GetActiveGrayScale()
{
	return cGrayScaleMap_->active;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::GrayScale::SetActiveGrayScale(bool active)
{
	cGrayScaleMap_->active = active;
}
