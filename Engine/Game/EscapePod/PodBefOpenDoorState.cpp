#include "PodBefOpenDoorState.h"
#include "EscapePod.h"
#include "CameraManager.h"
#include "Easing.h"
#include "PodOpenDoorState.h"

using namespace Easing;
using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PodBefOpenDoorState::Initialize()
{
	camera = CameraManager::GetInstance()->GetMainCamera();
	startEye_ = camera->GetEye();
	startTarget_ = camera->GetTarget();

	sPod_->GetAnimation()->SetAnimeName("PrevOpen");

	counter_.Initialize(cameraMoveFrame_, true);
	counter_.StartCount();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PodBefOpenDoorState::Update()
{
	counter_.Update();

	ShakeUpdate();

	CameraMoveUpdate();
}

void PodBefOpenDoorState::ShakeUpdate()
{
	if (counter_.GetFrameCount() > shakeFrame_) return;

	float t = counter_.GetFrameCount() / static_cast<float>(shakeFrame_);
	float shakeValue = EaseOut(maxShakeV_, 0.0f, t, Double);
	camera->SetShake(-shakeValue, shakeValue);

	sPod_->GetAnimation()->SetAnimeTimer(static_cast<float>(counter_.GetFrameCount()));

	if (counter_.GetFrameCount() == shakeFrame_) {
		camera->StopShake();
	}
}

void PodBefOpenDoorState::CameraMoveUpdate()
{
	camera->SetEye(EaseOut(startEye_, endEye_, counter_.GetCountPerMaxCount(), Triple));
	Vector3D target = startTarget_;
	target.y = lerp(startTarget_.y, endTargetY_, counter_.GetCountPerMaxCount());
	camera->SetTarget(target);

	if (counter_.GetFrameCount() == cameraMoveFrame_) {
		std::unique_ptr<EscPodState> next_ = std::make_unique<PodOpenDoorState>();
		sPod_->SetNextState(next_);
	}
}
