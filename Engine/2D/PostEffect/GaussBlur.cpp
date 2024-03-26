#include "GaussBlur.h"
#include "DirectX.h"
#include "ConstBuffStruct.h"
#include "PostEffectManager.h"
#include "Shader.h"
#include "PipelineManager.h"
#include <cassert>

using namespace MyMath;
using namespace MNE;

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////GaussBlurPostEffect//////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void MNE::GaussBlurPostEffect::Draw(int32_t /*mode*/)
{
	SetGPipelineAndIAVertIdxBuff();

	int32_t rootParaIdx = 0;
	SetGraphicsRoot(rootParaIdx);
	parent_->SetWeightGraphicsRootCBuffView(rootParaIdx++);

	originalPE_->DrawIndexedInstanced();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::GaussBlurPostEffect::SetGaussBlur(GaussBlur* gaussBlur)
{
	parent_ = gaussBlur;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////GaussBlur///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void MNE::GaussBlur::Initialize(IPostEffect* original)
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
		blurX->Initialize(width, height, original->GetName() + "/xBlur", 1, original->GetFormat());
		blurX->SetOriginalPostEffect(original);
		blurX->SetGaussBlur(this);

		blur = std::move(blurX);
		blurX_ = peMan->AddPostEffectBack(blur);
	}

	if (blurY_ == nullptr)
	{
		int32_t width = original->GetWidth() / 2;
		int32_t height = original->GetHeight() / 2;
		std::unique_ptr<GaussBlurPostEffect> blurY = std::make_unique<GaussBlurPostEffect>();
		blurY->Initialize(width, height, original->GetName() + "/yBlur", 1, original->GetFormat());
		blurY->SetOriginalPostEffect(blurX_);
		blurY->SetGaussBlur(this);

		blur = std::move(blurY);

		blurY_ = peMan->AddPostEffectBack(blur);
	}

	AddPipeline(original->GetFormat());
}

void MNE::GaussBlur::AddPipeline(DXGI_FORMAT format)
{
	PipelineManager* pipeMan = PipelineManager::GetInstance();

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	//	xyz座標
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }		//	uv座標
	};
	std::unique_ptr<GPipeline> pipeline;
	GPipeline* pipeX = nullptr;
	GPipeline* pipeY = nullptr;

	//	XBlur
	std::string name = "GaussBlurX" + std::to_string(format);

	pipeX = pipeMan->GetPipeline(name);

	if (pipeX == nullptr)
	{
		Shader xBlur("XBlurVS", "BlurPS");

		pipeline = std::make_unique<GPipeline>();
		pipeline->Initialize(xBlur, inputLayout, 2,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK,
			D3D12_DEPTH_WRITE_MASK_ZERO, true, format);

		pipeX = pipeMan->AddPipeline(pipeline, "GaussBlurX" + std::to_string(format));
	}

	if (blurX_ != nullptr)
	{
		blurX_->SetGPipeline(pipeX);
	}

	if (pipeY == nullptr)
	{
		//	YBlur
		Shader yBlur("YBlurVS", "BlurPS");

		pipeline = std::make_unique<GPipeline>();
		pipeline->Initialize(yBlur, inputLayout, 2,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK,
			D3D12_DEPTH_WRITE_MASK_ZERO, true, format);

		pipeY = pipeMan->AddPipeline(pipeline, "GaussBlurY" + std::to_string(format));
	}

	if (blurY_ != nullptr)
	{
		blurY_->SetGPipeline(pipeY);
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

Texture* MNE::GaussBlur::GetBlurredTexture()
{
	return blurY_->GetTexture();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

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
