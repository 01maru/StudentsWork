#include "GaussBlur.h"
#include "DirectX.h"
#include "ConstBuffStruct.h"
#include "PostEffect.h"
#include "TextureManager.h"
#include <cassert>

using namespace MyMath;

void MNE::GaussBlur::Initialize(float weight, MNE::PostEffect* original, DXGI_FORMAT format)
{
#pragma region ConstBuff

	weight_.Initialize((sizeof(CBuff::CBufferBlurWeight) + 0xFF) & ~0xFF);

	weights_.resize(8);
	MyMath::CalcGaussianWeightsTable(
		weights_,			// 重みの格納先
		weight				// ボケ具合。この数値が大きくなるとボケが強くなる
	);

	CBuff::CBufferBlurWeight* mapWeight = nullptr;
	HRESULT result = weight_.GetResource()->Map(0, nullptr, (void**)&mapWeight);	//	マッピング
	mapWeight->weight[0] = Vector4D(weights_[0], weights_[1], weights_[2], weights_[3]);
	mapWeight->weight[1] = Vector4D(weights_[4], weights_[5], weights_[6], weights_[7]);
	assert(SUCCEEDED(result));
	weight_.GetResource()->Unmap(0, nullptr);

#pragma endregion

	original_ = original;

	int32_t width = original_->GetWidth() / 2;
	int32_t height = original_->GetHeight();
	blurX_ = std::make_unique<MNE::PostEffect>();
	blurX_->Initialize(width, height, original_->GetName() + "/xblur", 1, format);

	height /= 2;
	blurY_ = std::make_unique<MNE::PostEffect>();
	blurY_->Initialize(width, height, original_->GetName() + "/yblur", 1, format);
}

void MNE::GaussBlur::Draw()
{
	MyDirectX* dx = MyDirectX::GetInstance();


	dx->PrevPostEffect(blurX_.get(), blurX_->GetClearColor());

	blurX_->SetGPipelineAndIAVertIdxBuff(pipeline[0]);
	weight_.SetGraphicsRootCBuffView(2);

	original_->Draw();

	dx->PostEffectDraw(blurX_.get());

	dx->PrevPostEffect(blurY_.get(), blurY_->GetClearColor());

	blurX_->SetGPipelineAndIAVertIdxBuff(pipeline[1]);

	dx->GetCmdList()->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(blurX_->GetTexture()->GetHandle()));
	weight_.SetGraphicsRootCBuffView(2);

	blurX_->DrawIndexedInstanced();

	dx->PostEffectDraw(blurY_.get());
}

void MNE::GaussBlur::SetPipeline(GPipeline* blurXPipeline, GPipeline* blurYPipeline)
{
	pipeline[0] = blurXPipeline;
	pipeline[1] = blurYPipeline;
}

void MNE::GaussBlur::SetClearColor(const Vector4D& color)
{
	blurX_->SetColor(color);
	blurY_->SetColor(color);
}
