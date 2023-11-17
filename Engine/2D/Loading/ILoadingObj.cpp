#include "ILoadingObj.h"

void ILoadingObj::Update()
{
    counter_.Update();

    drawObj_ = counter_.GetFrameCount() > 0;
}

void ILoadingObj::SetIsLoading(bool loading)
{
    loading_ = loading;
    counter_.SetIsIncrement(loading);
    counter_.StartCount();
}

void ILoadingObj::SetFadeAnimeTime(int32_t time)
{
    counter_.Initialize(time, loading_);
}
