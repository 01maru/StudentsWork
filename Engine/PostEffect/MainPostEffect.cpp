#include "MainPostEffect.h"
#include "PipelineManager.h"
#include "TextureManager.h"
#include "DirectX.h"
#include "SceneManager.h"

#include "Shader.h"

using namespace MNE;

void MNE::MainPostEffect::Initialize(int32_t width, int32_t height, const std::string& name, int32_t textureNum, DXGI_FORMAT format)
{
	PipelineManager* pipeMan = PipelineManager::GetInstance();
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	//	xyz座標
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }		//	uv座標
	};			

	Shader shader("ScreenVS", "ScreenPS");

	std::unique_ptr<GPipeline> pipeline = std::make_unique<GPipeline>();
	pipeline->Initialize(shader, inputLayout, 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO,
		true, DXGI_FORMAT_R11G11B10_FLOAT, 4);

	pipeline_ = pipeMan->AddPipeline(pipeline, "MainPostEffect");

	//	Luminance
	Shader luminanceShader("ScreenVS", "LuminancePS");

	pipeline = std::make_unique<GPipeline>();
	pipeline->Initialize(luminanceShader, inputLayout, 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO);

	luminancePipe_ = pipeMan->AddPipeline(pipeline, "Luminance");

	IPostEffect::Initialize(width, height, name, textureNum, format);
}

void MNE::MainPostEffect::Update()
{
	MyDirectX* dx = MyDirectX::GetInstance();

	dx->PrevPostEffect(this, clearColor_);

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
	cmdList->SetGraphicsRootDescriptorTable(rootParaIdx++, TextureManager::GetInstance()->GetTextureHandle(texture_[1]->GetHandle()));
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
	TextureManager* texMan = TextureManager::GetInstance();
	cmdList->SetGraphicsRootDescriptorTable(rootParaIdx++, texMan->GetTextureHandle(texture_[0]->GetHandle()));
	cmdList->SetGraphicsRootDescriptorTable(rootParaIdx++, texMan->GetTextureHandle(luminanceTex_[0]->GetHandle()));
	cmdList->SetGraphicsRootDescriptorTable(rootParaIdx++, texMan->GetTextureHandle(luminanceTex_[0]->GetHandle()));
	cmdList->SetGraphicsRootDescriptorTable(rootParaIdx++, texMan->GetTextureHandle(luminanceTex_[0]->GetHandle()));
	material_.SetGraphicsRootCBuffView(rootParaIdx++);

	PlanePolygon::DrawIndexedInstanced();
}

void MNE::MainPostEffect::SetLuminanceTex(int32_t idx, Texture* tex)
{
	luminanceTex_[idx] = tex;
}
