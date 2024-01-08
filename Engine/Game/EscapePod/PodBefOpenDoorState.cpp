#include "PodBefOpenDoorState.h"
#include "EscapePod.h"
#include "CameraManager.h"
#include "Easing.h"
#include "PodCameraMoveState.h"

using namespace Easing;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PodBefOpenDoorState::Initialize()
{
	camera = CameraManager::GetInstance()->GetMainCamera();
	
	counter_.Initialize(shakeFrame_, true);
	counter_.StartCount();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PodBefOpenDoorState::Update()
{
	counter_.Update();

	float shakeValue = EaseOut(0.1f, 0.0f, counter_.GetCountPerMaxCount(), 2);
	camera->SetShake(-shakeValue, shakeValue);

	if (counter_.GetFrameCount() == shakeFrame_) {
		camera->StopShake();
		std::unique_ptr<EscPodState> next_ = std::make_unique<PodCameraMoveState>();
		sPod_->SetNextState(next_);
	}
}
