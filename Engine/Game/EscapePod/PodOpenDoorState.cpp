#include "PodOpenDoorState.h"
#include "EscapePod.h"
#include "CameraManager.h"
#include "Easing.h"

using namespace Easing;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PodOpenDoorState::Initialize()
{
	camera = CameraManager::GetInstance()->GetMainCamera();
	startEye_ = camera->GetEye();
	startTarget_ = camera->GetTarget();

	counter_.Initialize(openDoorFrame_, true);
	counter_.StartCount();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PodOpenDoorState::Update()
{
	counter_.Update();

	float len = EaseOutBack(0.0f, moveZ, counter_.GetCountPerMaxCount());
	Vector3D eye = startEye_;
	eye.z += len;
	camera->SetEye(eye);
	Vector3D target = startTarget_;
	target.z = len;
	camera->SetTarget(target);

	if (counter_.GetFrameCount() == openDoorFrame_) {
		std::unique_ptr<EscPodState> next_ = std::make_unique<EscPodState>();
		sPod_->SetNextState(next_);
	}
}
