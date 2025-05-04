#include "LoadingSprite.h"
#include "PipelineManager.h"
#include "Easing.h"
#include "Shader.h"

using namespace Easing;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void MNE::LoadingSprite::Initialize()
{
	Sprite::Initialize();

    Sprite::SetColor(color_);

#pragma region Pipeline

	PipelineManager* pipeMan = PipelineManager::GetInstance();

	pipeline_ = pipeMan->GetPipeline("LoadingSprite");

	if (pipeline_ == nullptr)
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },	//	xyz座標
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }		//	uv座標
		};

		Shader shader("SpriteVS", "SpritePS");

		std::unique_ptr<GPipeline> pipeline = std::make_unique<GPipeline>();
		pipeline->Initialize(shader, inputLayout, 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
			D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK_ZERO, 
			true, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
		pipeline->SetBlendMode(Blend::ALPHA_BLEND);

		pipeline_ = pipeMan->AddPipeline(pipeline, "LoadingSprite");
	}

#pragma endregion
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void MNE::LoadingSprite::Update()
{
    ILoadingObj::Update();

    float alphaColor;
    float minAlpha = 0.0f;
    float maxAlpha = 1.0f;
    alphaColor = EaseOut(minAlpha, maxAlpha, counter_.GetCountPerMaxCount(), easePaw_);

    Sprite::SetAlphaColor(alphaColor);

    timer_.Update();

    float size = EaseOut(maxSize_, minSize_, timer_.GetCountPerMaxCount(), sizeEasePaw_);
    Sprite::SetSize(Vector2D(size, size));

    Sprite::Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void MNE::LoadingSprite::Draw()
{
    Sprite::Draw(pipeline_);
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::LoadingSprite::Start()
{
    Reset();
    timer_.StartCount();
}

void MNE::LoadingSprite::Reset()
{
    timer_.Initialize(time_, true, true, countNum_);
    Sprite::SetSize(Vector2D(maxSize_, maxSize_));
    Sprite::Update();
}

void MNE::LoadingSprite::SetMaxSize(float size)
{
    maxSize_ = size;
}
