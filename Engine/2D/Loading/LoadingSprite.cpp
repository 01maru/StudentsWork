#include "LoadingSprite.h"
#include "PipelineManager.h"
#include "Easing.h"

using namespace Easing;

void MNE::LoadingSprite::Initialize()
{
	Sprite::Initialize();

    Sprite::SetColor(color_);
}

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

void MNE::LoadingSprite::Draw()
{
    Sprite::Draw(PipelineManager::GetInstance()->GetPipeline("LoadingSprite"));
}

void MNE::LoadingSprite::Start()
{
    Reset();
    timer_.StartCount();
}

void MNE::LoadingSprite::Reset()
{
    timer_.Initialize(10, true, true, 2);
    Sprite::SetSize(Vector2D(maxSize_, maxSize_));
    Sprite::Update();
}

void MNE::LoadingSprite::SetMaxSize(float size)
{
    maxSize_ = size;
}
