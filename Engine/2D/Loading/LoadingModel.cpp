#include "LoadingModel.h"
#include "CameraManager.h"
#include "Easing.h"

using namespace Easing;

void LoadingModel::Initialize()
{
    Object3DShilhouette::Initialize();

    Object3DShilhouette::SetCamera(CameraManager::GetInstance()->GetOrthoProjCamera());
}

void LoadingModel::Update()
{
    ILoadingObj::Update();

    float alphaColor;
    float minAlpha = 0.0f;
    float maxAlpha = 1.0f;
    alphaColor = EaseOut(minAlpha, maxAlpha, counter_.GetCountPerMaxCount(), easePaw_);

    Object3DShilhouette::SetAlphaColor({ alphaColor });

    //  モデル表示中は回転させる
    Vector3D rot = Object3DShilhouette::GetRotation();
    rot.y -= rotSpd_;
    Object3DShilhouette::SetRotation(rot);
    Object3DShilhouette::MatUpdate();
}

void LoadingModel::Draw()
{
    Object3DShilhouette::Draw();
}
