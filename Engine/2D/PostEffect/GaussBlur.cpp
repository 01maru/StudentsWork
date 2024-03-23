#include "GaussBlur.h"
#include "DirectX.h"
#include "ConstBuffStruct.h"
#include "TextureManager.h"
#include <cassert>

#include "PostEffectManager.h"

#include "PipelineManager.h"

using namespace MyMath;
using namespace MNE;

void MNE::GaussBlurPostEffect::Draw(int32_t /*mode*/)
{
	SetGPipelineAndIAVertIdxBuff();
	parent_->SetWeightGraphicsRootCBuffView(2);

	originalPE_->Draw();
}

void MNE::GaussBlurPostEffect::SetGaussBlur(GaussBlur* gaussBlur)
{
	parent_ = gaussBlur;
}

/////////////////

void MNE::GaussBlur::Initialize(IPostEffect* original, DXGI_FORMAT format)
{
#pragma region ConstBuff

	weight_.Initialize((sizeof(CBuff::CBufferBlurWeight) + 0xFF) & ~0xFF);

	HRESULT result = weight_.GetResource()->Map(0, nullptr, (void**)&mapWeight_);	//	マッピング
	assert(SUCCEEDED(result));

#pragma endregion

	PostEffectManager* peMan = PostEffectManager::GetInstance();

	auto originItr = peMan->GetPostEffectItr(original->GetName());

	std::unique_ptr<IPostEffect> blur;
	if (blurX_ == nullptr)
	{
		int32_t width = original->GetWidth() / 2;
		int32_t height = original->GetHeight();
		std::unique_ptr<GaussBlurPostEffect> blurX = std::make_unique<GaussBlurPostEffect>();
		blurX->Initialize(width, height, original->GetName() + "/xBlur", 1, format);
		blurX->SetOriginalPostEffect(original);
		blurX->SetGaussBlur(this);

		blurX->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		blurX->SetGPipeline(PipelineManager::GetInstance()->GetPipeline("luminncexBlur"));

		blur = std::move(blurX);
		//IPostEffect* blurXPtr = peMan->AddPostEffectBack(blur, std::next(originItr));
		IPostEffect* blurXPtr = peMan->AddPostEffectBack(blur);
		blurX_ = dynamic_cast<GaussBlurPostEffect*>(blurXPtr);
	}

	if (blurY_ == nullptr)
	{
		int32_t width = original->GetWidth() / 2;
		int32_t height = original->GetHeight() / 2;
		std::unique_ptr<GaussBlurPostEffect> blurY = std::make_unique<GaussBlurPostEffect>();
		blurY->Initialize(width, height, original->GetName() + "/yBlur", 1, format);
		blurY->SetOriginalPostEffect(blurX_);
		blurY->SetGaussBlur(this);

		blurY->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		blurY->SetGPipeline(PipelineManager::GetInstance()->GetPipeline("luminnceyBlur"));

		blur = std::move(blurY);

		//IPostEffect* blurYPtr = peMan->AddPostEffect(blur, std::next(std::next(originItr)));
		IPostEffect* blurYPtr = peMan->AddPostEffectBack(blur);
		tex = blurYPtr->GetTexture();
		blurY_ = dynamic_cast<GaussBlurPostEffect*>(blurYPtr);
	}
}

void MNE::GaussBlur::SetWeightGraphicsRootCBuffView(int32_t rootparaIdx)
{
	weight_.SetGraphicsRootCBuffView(rootparaIdx);
}

void MNE::GaussBlur::SetWeight(float weight)
{
	CalcGaussianWeightsTable(
		weights_,			// 重みの格納先
		weight				// ボケ具合。この数値が大きくなるとボケが強くなる
	);
	//	格納
	mapWeight_->weight[0] = Vector4D(weights_[0], weights_[1], weights_[2], weights_[3]);
	mapWeight_->weight[1] = Vector4D(weights_[4], weights_[5], weights_[6], weights_[7]);
}

void MNE::GaussBlur::SetPipeline(GPipeline* blurXPipeline, GPipeline* blurYPipeline)
{
	blurX_->SetGPipeline(blurXPipeline);
	blurY_->SetGPipeline(blurYPipeline);
}

void MNE::GaussBlur::SetClearColor(const MyMath::Vector4D& color)
{
	blurX_->SetClearColor(color);
	blurY_->SetClearColor(color);
}
