#include "MainPostEffect.h"
#include "PipelineManager.h"
#include "TextureManager.h"
#include "DirectX.h"
#include "SceneManager.h"

using namespace MNE;

void MNE::MainPostEffect::Initialize(int32_t width, int32_t height, const std::string& name, int32_t textureNum, DXGI_FORMAT format)
{
	pipeline_ = PipelineManager::GetInstance()->GetPipeline("PostEffect");
	luminancePipe_ = PipelineManager::GetInstance()->GetPipeline("Luminnce", Blend::NONE_BLEND);

	IPostEffect::Initialize(width, height, name, textureNum, format);
}

void MNE::MainPostEffect::Update()
{
	MyDirectX* dx = MyDirectX::GetInstance();

	dx->PrevPostEffect(this);

	SceneManager::GetInstance()->DrawScene();

	if (originalPE_ != nullptr)
	{
		originalPE_->Draw();
	}

	dx->PostEffectDraw(this);
}

void MNE::MainPostEffect::Draw(int32_t mode)
{
	switch (mode)
	{
	case Normal:

		DrawNormal();

		break;
	case Luminance:

		DrawLuminance();

		break;
	default:
		break;
	}

}

void MNE::MainPostEffect::DrawLuminance()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	int32_t rootParaIdx = 0;

	luminancePipe_->SetGraphicsRootSignature();
	luminancePipe_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertIdxBuff::IASetVertIdxBuff();
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(rootParaIdx++, TextureManager::GetInstance()->GetTextureHandle(texture_[0]->GetHandle()));
	material_.SetGraphicsRootCBuffView(rootParaIdx++);

	PlanePolygon::DrawIndexedInstanced();
}

void MNE::MainPostEffect::DrawNormal()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	int32_t rootParaIdx = 0;

	pipeline_->SetGraphicsRootSignature();
	pipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertIdxBuff::IASetVertIdxBuff();

	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(rootParaIdx++, TextureManager::GetInstance()->GetTextureHandle(texture_[0]->GetHandle()));
	//if (handle1 != -1) {
	//	cmdList->SetGraphicsRootDescriptorTable(rootParaIdx++, TextureManager::GetInstance()->GetTextureHandle(handle1));
	//}
	//else {
	//	
	//}
	cmdList->SetGraphicsRootDescriptorTable(rootParaIdx++, TextureManager::GetInstance()->GetTextureHandle(texture_[0]->GetHandle()));
	cmdList->SetGraphicsRootDescriptorTable(rootParaIdx++, TextureManager::GetInstance()->GetTextureHandle(texture_[0]->GetHandle()));
	cmdList->SetGraphicsRootDescriptorTable(rootParaIdx++, TextureManager::GetInstance()->GetTextureHandle(texture_[0]->GetHandle()));
	material_.SetGraphicsRootCBuffView(rootParaIdx++);

	PlanePolygon::DrawIndexedInstanced();
}
