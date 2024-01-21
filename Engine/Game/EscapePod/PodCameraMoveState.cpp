#include "PodCameraMoveState.h"
#include "EscapePod.h"
#include "CameraManager.h"
#include "Easing.h"
#include "PodOpenDoorState.h"

using namespace Easing;
using namespace MNE;

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

	sPod_->GetPtrLetterBox()->ResetAnimation(false);

	sPod_->GetAnimation()->SetAnimeName("AfterOpen");
	sPod_->GetAnimation()->SetAutoPlay(true);
	sPod_->GetAnimation()->SetAnimeTimer(0.0f);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PodCameraMoveState::Update()
{
	counter_.Update();

	camera->SetEye(EaseIn(startEye_, endEye_, counter_.GetCountPerMaxCount(), Single));
	camera->SetTarget(EaseIn(startTarget_, endTarget_, counter_.GetCountPerMaxCount(), Single));

	if (counter_.GetFrameCount() == cameraMoveFrame_) {
		std::unique_ptr<EscPodState> next_ = std::make_unique<EscPodState>();
		sPod_->SetNextState(next_);
		sPod_->SetOpenDoor(true);
	}
}
