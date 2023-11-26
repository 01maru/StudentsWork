#include "LoadingSprite.h"
#include "PipelineManager.h"
#include "Easing.h"

using namespace Easing;

void LoadingSprite::Initialize()
{
	Sprite::Initialize();
}

void LoadingSprite::Update()
{
    ILoadingObj::Update();

    float alphaColor;
    float minAlpha = 0.0f;
    float maxAlpha = 1.0f;
    alphaColor = EaseOut(minAlpha, maxAlpha, counter_.GetCountPerMaxCount(), easePaw_);

    Sprite::SetAlphaColor(alphaColor);

    float rot = Sprite::GetRotation();
    Sprite::SetRotation(rot - rotSpd_);
    Sprite::Update();
}

void LoadingSprite::Draw()
{
    Sprite::Draw(PipelineManager::GetInstance()->GetPipeline("LoadingSprite"));
}
