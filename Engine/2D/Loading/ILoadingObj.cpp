#include "ILoadingObj.h"

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void MNE::ILoadingObj::Update()
{
    counter_.Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::ILoadingObj::SetIsLoading(bool loading)
{
    loading_ = loading;
    counter_.SetIsIncrement(loading);
    counter_.StartCount();
}

void MNE::ILoadingObj::SetFadeAnimeTime(int32_t time)
{
    counter_.Initialize(time, loading_);
}
