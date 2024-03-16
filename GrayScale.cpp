#include "GrayScale.h"
#include "ConstBuffStruct.h"
#include "DirectX.h"
#include "TextureManager.h"
#include "PipelineManager.h"
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

	pipeline_ = PipelineManager::GetInstance()->GetPipeline("glayScale");

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
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::GrayScale::SetActiveGrayScale(bool active)
{
	cGrayScaleMap_->active = active;
}
