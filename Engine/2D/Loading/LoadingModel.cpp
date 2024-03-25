#include "LoadingModel.h"
#include "CameraManager.h"
#include "Easing.h"

using namespace Easing;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void MNE::LoadingModel::Initialize()
{
    Object3DSilhouette::Initialize();

    Object3DSilhouette::SetCamera(CameraManager::GetInstance()->GetOrthoProjCamera());
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void MNE::LoadingModel::Update()
{
    ILoadingObj::Update();

    float alphaColor;
    float minAlpha = 0.0f;
    float maxAlpha = 1.0f;
    alphaColor = EaseOut(minAlpha, maxAlpha, counter_.GetCountPerMaxCount(), easePaw_);

    Object3DSilhouette::SetAlphaColor({ alphaColor });

    //  モデル表示中は回転させる
    Vector3D rot = Object3DSilhouette::GetRotation();
    rot.y -= rotSpd_;
    Object3DSilhouette::SetRotation(rot);
    Object3DSilhouette::MatUpdate();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void MNE::LoadingModel::Draw()
{
    Object3DSilhouette::Draw();
}
