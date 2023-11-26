#include "ILoadingObj.h"

void ILoadingObj::Update()
{
    counter_.Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

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
