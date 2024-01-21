#include "PodOpenDoorState.h"
#include "EscapePod.h"
#include "CameraManager.h"
#include "Easing.h"
#include "PodCameraMoveState.h"

using namespace Easing;
using namespace MNE;

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
	move_ = startTarget_ - startEye_;
	move_.y = 0.0f;

	sPod_->GetAnimation()->SetAnimeName("Open");
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PodOpenDoorState::Update()
{
	counter_.Update();

	//int32_t animeT = MyMath::mMin(counter_.GetFrameCount() * 3, 88);
	//sPod_->GetAnimation()->SetAnimatonTimer(static_cast<float>(animeT));

	sPod_->GetAnimation()->SetAnimeTimer(static_cast<float>(counter_.GetFrameCount()));

	Vector3D move = EaseIn(Vector3D(), move_, counter_.GetCountPerMaxCount(), Double);

	Vector3D eye = startEye_ + move;
	camera->SetEye(eye);

	Vector3D target = startTarget_ + move;
	camera->SetTarget(target);

	if (counter_.GetIsActive() == FALSE) {
		sPod_->SetDrawPlayer(true);
		std::unique_ptr<EscPodState> next_ = std::make_unique<PodCameraMoveState>();
		sPod_->SetNextState(next_);
	}
}
