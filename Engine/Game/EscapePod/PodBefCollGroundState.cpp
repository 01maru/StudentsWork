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

	//	脱出ポッドの座標とターゲットの距離
	targetToPod_ = camera->GetTarget() - sPod_->GetPosition();
	endPosY_ = sPod_->GetPosition().y;

	sPod_->GetPtrLetterBox()->ResetAnimation(true);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PodBefCollGroundState::Update()
{
	counter_.Update();

	Vector3D pos = sPod_->GetPosition();
	pos.y = EaseIn(startPosY_, endPosY_, counter_.GetCountPerMaxCount(), Quint);
	sPod_->SetPosition(pos);

	camera->SetTarget(pos + targetToPod_);

	float shakeValue = EaseIn(0.0f, maxShakeV_, counter_.GetCountPerMaxCount(), Double);
	camera->SetShake(-shakeValue, shakeValue);

	if (counter_.GetFrameCount() == moveMaxFrame_) {
		std::unique_ptr<EscPodState> next_ = std::make_unique<PodCollGroundState>();
		sPod_->SetNextState(next_);
	}
}
