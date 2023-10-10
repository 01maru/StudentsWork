#include "DissolveSprite.h"
#include "ConstBuffStruct.h"
#include <cassert>

#include "PipelineManager.h"
#include "TextureManager.h"
#include "DirectX.h"

void DissolveSprite::Initialize(Texture* texture)
{
	Sprite::Initialize(texture);

#pragma region ConstBuffer

	cbDissolve_.Initialize(sizeof(CBuff::CBuffSpriteTransform));
	//	定数バッファのマッピング
	HRESULT result = cbDissolve_.GetResource()->Map(0, nullptr, (void**)&cbDissolveMat_);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion
}

void DissolveSprite::Update()
{
	Sprite::Update();

	if (dirtyFlagDissolve_)
	{
		dirtyFlagDissolve_ = false;

		cbDissolveMat_->color = dissolveColor_;
		cbDissolveMat_->value = disolveValue_;
	}
}

void DissolveSprite::Draw(GPipeline* pipeline)
{
	if (isInvisible_) return;

	GPipeline* pipeline_ = nullptr;
	if (pipeline != nullptr) pipeline_ = pipeline;

	else					 pipeline_ = PipelineManager::GetInstance()->GetPipeline("DissolveSprite", GPipeline::ALPHA_BLEND);
	pipeline_->SetGraphicsRootSignature();
	pipeline_->SetPipeStateAndPrimitive(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	IASetVertIdxBuff();
	//	テクスチャ
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(handle_->GetHandle()));

	Texture* dissolve = TextureManager::GetInstance()->LoadTextureGraph("fogTex.jpg");
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetTextureHandle(dissolve->GetHandle()));

	cbColorMaterial_.SetGraphicsRootCBuffView(2);
	cbTransform_.SetGraphicsRootCBuffView(3);
	cbDissolve_.SetGraphicsRootCBuffView(4);

	MyDirectX::GetInstance()->GetCmdList()->DrawInstanced(4, 1, 0, 0);
}

void DissolveSprite::SetDissolveValue(float value)
{
	if (disolveValue_ != value)
	{
		dirtyFlagDissolve_ = true;
		disolveValue_ = value;
	}
}
