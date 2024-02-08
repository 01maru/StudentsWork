#include "LoadingSprite.h"
#include "PipelineManager.h"
#include "Easing.h"

using namespace Easing;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void MNE::LoadingSprite::Initialize()
{
	Sprite::Initialize();

    Sprite::SetColor(color_);
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
    Sprite::Draw(PipelineManager::GetInstance()->GetPipeline("LoadingSprite"));
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
