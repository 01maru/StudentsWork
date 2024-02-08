#include "PodBefOpenDoorState.h"
#include "EscapePod.h"
#include "CameraManager.h"
#include "Easing.h"
#include "PodOpenDoorState.h"

using namespace Easing;
using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PodBefOpenDoorState::Initialize()
{
	//	カメラの初期位置設定
	camera = CameraManager::GetInstance()->GetMainCamera();
	startEye_ = camera->GetEye();
	startTarget_ = camera->GetTarget();

	//	アニメーションをドアを開ける前の動きに設定
	sPod_->GetAnimation()->SetAnimeName("PrevOpen");

	//	カウンタースタート
	counter_.Initialize(cameraMoveFrame_, TRUE);
	counter_.StartCount();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PodBefOpenDoorState::ShakeUpdate()
{
	//	カウンターがシェイクタイマー範囲外だったら処理しない
	if (counter_.GetFrameCount() > shakeFrame_) return;

	//	シェイク処理
	float t = counter_.GetFrameCount() / static_cast<float>(shakeFrame_);
	const float noShakeV = 0.0f;
	float shakeValue = EaseOut(maxShakeV_, noShakeV, t, Double);
	camera->SetShake(-shakeValue, shakeValue);

	//	ドアを開ける前のアニメーション処理
	sPod_->GetAnimation()->SetAnimeTimer(static_cast<float>(counter_.GetFrameCount()));

	//	カウントが終わったらカメラのシェイクを止める
	if (counter_.GetFrameCount() == shakeFrame_) {
		camera->StopShake();
	}
}

void PodBefOpenDoorState::CameraMoveUpdate()
{
	//	視点移動
	camera->SetEye(EaseOut(startEye_, endEye_, counter_.GetCountPerMaxCount(), Triple));
	//	ターゲット移動
	Vector3D target = startTarget_;
	target.y = lerp(startTarget_.y, endTargetY_, counter_.GetCountPerMaxCount());
	camera->SetTarget(target);

	//	カウントが終わったら次のステートへ
	if (counter_.GetFrameCount() == cameraMoveFrame_) {
		std::unique_ptr<EscPodState> next_ = std::make_unique<PodOpenDoorState>();
		sPod_->SetNextState(next_);
	}
}

void PodBefOpenDoorState::Update()
{
	//	カウンター更新
	counter_.Update();

	ShakeUpdate();

	CameraMoveUpdate();
}
