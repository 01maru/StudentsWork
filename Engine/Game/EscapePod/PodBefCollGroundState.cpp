#include "PodBefCollGroundState.h"
#include "EscapePod.h"
#include "CameraManager.h"
#include "Easing.h"
#include "PodCollGroundState.h"

using namespace Easing;
using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PodBefCollGroundState::Initialize()
{
	counter_.Initialize(moveMaxFrame_, true);
	counter_.StartCount();

	camera = CameraManager::GetInstance()->GetMainCamera();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PodBefCollGroundState::Update()
{
	counter_.Update();

	Vector3D pos = sPod_->GetPosition();
	pos.y = EaseIn(startPosY_, -0.3f, counter_.GetCountPerMaxCount(), 5);
	sPod_->SetPosition(pos);

	camera->SetTarget(pos + Vector3D(0.0f, 2.0f, 0.0f));

	float shakeValue = EaseIn(0.0f, 0.5f, counter_.GetCountPerMaxCount(), 2);
	camera->SetShake(-shakeValue, shakeValue);

	if (counter_.GetFrameCount() == moveMaxFrame_) {
		std::unique_ptr<EscPodState> next_ = std::make_unique<PodCollGroundState>();
		sPod_->SetNextState(next_);
	}
}
