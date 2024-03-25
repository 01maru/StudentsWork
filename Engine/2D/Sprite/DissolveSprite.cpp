#include "DissolveSprite.h"
#include "ConstBuffStruct.h"
#include "PipelineManager.h"
#include "TextureManager.h"
#include "DirectX.h"
#include "RootParameterIdx.h"
#include "Shader.h"
#include <cassert>

void MNE::DissolveSprite::Initialize(Texture* texture)
{
	Sprite::Initialize(texture);

#pragma region ConstBuffer

	cbDissolve_.Initialize(sizeof(CBuff::CBuffSpriteTransform));
	//	定数バッファのマッピング
	HRESULT result = cbDissolve_.GetResource()->Map(0, nullptr, (void**)&cbDissolveMat_);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region Pipeline

	PipelineManager* pipeMan = PipelineManager::GetInstance();

	pipeline_ = pipeMan->GetPipeline("DissolveSprite");

	if (pipeline_ == nullptr)
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	//	xyz座標
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }		//	uv座標
		};

		Shader shader("SpriteVS", "DissolveSpritePS");

		std::unique_ptr<GPipeline> pipeline = std::make_unique<GPipeline>();
		pipeline->Initialize(shader, inputLayout, 3, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
			D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO,
			true, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 2);
		pipeline->SetBlendMord(Blend::ALPHA_BLEND);

		pipeline_ = pipeMan->AddPipeline(pipeline, "DissolveSprite");
	}

#pragma endregion
}

void MNE::DissolveSprite::Update()
{
	Sprite::Update();

	if (dirtyFlagDissolve_)
	{
		dirtyFlagDissolve_ = false;

		cbDissolveMat_->color = dissolveColor_;
		cbDissolveMat_->value = dissolveValue_;
	}
}

void MNE::DissolveSprite::Draw(GPipeline* pipeline)
{
	if (isInvisible_) return;

	GPipeline* pipe = nullptr;
	if (pipeline != nullptr) pipe = pipeline;

	else					 pipe = pipeline_;
	pipe->SetGraphicsRootSignature();
	pipe->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	IASetVertIdxBuff();

	int32_t nextIdx = MNE::Zero;
	//	テクスチャ
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(nextIdx++, TextureManager::GetInstance()->GetTextureHandle(handle_->GetHandle()));

	Texture* dissolve = TextureManager::GetInstance()->GetTextureGraph("noise.png");
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(nextIdx++, TextureManager::GetInstance()->GetTextureHandle(dissolve->GetHandle()));

	cbColorMaterial_.SetGraphicsRootCBuffView(nextIdx++);
	cbTransform_.SetGraphicsRootCBuffView(nextIdx++);
	cbDissolve_.SetGraphicsRootCBuffView(nextIdx++);

	MyDirectX::GetInstance()->GetCmdList()->DrawInstanced(VertexSize, 1, 0, 0);
}

void MNE::DissolveSprite::SetDissolveValue(float value)
{
	if (dissolveValue_ != value)
	{
		dirtyFlagDissolve_ = true;
		dissolveValue_ = value;
	}
}
