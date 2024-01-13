#include "DissolveSprite.h"
#include "ConstBuffStruct.h"
#include <cassert>

#include "PipelineManager.h"
#include "TextureManager.h"
#include "DirectX.h"
#include "RootParameterIdx.h"

void MNE::DissolveSprite::Initialize(Texture* texture)
{
	Sprite::Initialize(texture);

#pragma region ConstBuffer

	cbDissolve_.Initialize(sizeof(CBuff::CBuffSpriteTransform));
	//	定数バッファのマッピング
	HRESULT result = cbDissolve_.GetResource()->Map(0, nullptr, (void**)&cbDissolveMat_);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion
}

void MNE::DissolveSprite::Update()
{
	Sprite::Update();

	if (dirtyFlagDissolve_)
	{
		dirtyFlagDissolve_ = false;

		cbDissolveMat_->color = dissolveColor_;
		cbDissolveMat_->value = disolveValue_;
	}
}

void MNE::DissolveSprite::Draw(GPipeline* pipeline)
{
	if (isInvisible_) return;

	GPipeline* pipeline_ = nullptr;
	if (pipeline != nullptr) pipeline_ = pipeline;

	else					 pipeline_ = PipelineManager::GetInstance()->GetPipeline("DissolveSprite", Blend::ALPHA_BLEND);
	pipeline_->SetGraphicsRootSignature();
	pipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

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
	if (disolveValue_ != value)
	{
		dirtyFlagDissolve_ = true;
		disolveValue_ = value;
	}
}
