#include "GlayScale.h"
#include "DirectX.h"
#include "ConstBuffStruct.h"
#include "Window.h"
#include <cassert>
#include "PipelineManager.h"
#include "TextureManager.h"

void GlayScale::Initialize(PostEffect* original)
{
#pragma region  ConstBuffer

	activeGlay_.Initialize((sizeof(CBuff::CBuffGlayScale) + 0xFF) & ~0xFF);
	HRESULT result = activeGlay_.GetResource()->Map(0, nullptr, (void**)&cGlayScaleMap_);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion

	original_ = original;

	pipeline_ = PipelineManager::GetInstance()->GetPipeline("glayScale");

	PostEffect::Initialize(Window::sWIN_WIDTH, Window::sWIN_HEIGHT,"glay", 2, DXGI_FORMAT_R11G11B10_FLOAT);
}

void GlayScale::DrawGlay()
{
	SetGPipelineAndIAVertIdxBuff(pipeline_);
	activeGlay_.SetGraphicsRootCBuffView(2);

	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(GetTexture()->GetHandle()));

	PlanePolygon::DrawIndexedInstanced();
}

void GlayScale::SetGlayScale(bool active)
{
	cGlayScaleMap_->active = active;
}
