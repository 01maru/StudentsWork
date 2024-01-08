#include "PodCameraMoveState.h"
#include "EscapePod.h"
#include "CameraManager.h"
#include "Easing.h"
#include "PodOpenDoorState.h"

using namespace Easing;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PodCameraMoveState::Initialize()
{
	camera = CameraManager::GetInstance()->GetMainCamera();
	startEye_ = camera->GetEye();
	startTarget_ = camera->GetTarget();

	counter_.Initialize(cameraMoveFrame_, true);
	counter_.StartCount();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PodCameraMoveState::Update()
{
	counter_.Update();

	camera->SetEye(lerp(startEye_, endEye_, counter_.GetCountPerMaxCount()));
	Vector3D target = startTarget_;
	target.y = lerp(startTarget_.y, endTargetY_, counter_.GetCountPerMaxCount());
	camera->SetTarget(target);

	if (counter_.GetFrameCount() == cameraMoveFrame_) {
		std::unique_ptr<EscPodState> next_ = std::make_unique<PodOpenDoorState>();
		sPod_->SetNextState(next_);
	}
}
