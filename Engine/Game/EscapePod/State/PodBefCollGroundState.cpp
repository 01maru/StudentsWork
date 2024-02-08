#include "PodBefCollGroundState.h"
#include "EscapePod.h"
#include "CameraManager.h"
#include "Easing.h"
#include "PodCollGroundState.h"
#include "UIData.h"

using namespace Easing;
using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PodBefCollGroundState::Initialize()
{
	//	カウンター初期化
	counter_.Initialize(moveMaxFrame_, TRUE);
	counter_.StartCount();

	camera = CameraManager::GetInstance()->GetMainCamera();

	//	脱出ポッドの座標とターゲットの距離
	targetToPod_ = camera->GetTarget() - sPod_->GetPosition();
	endPosY_ = sPod_->GetPosition().y;

	//	ムービー用黒帯表示
	sPod_->GetLetterBoxPtr()->ResetAnimation(TRUE);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PodBefCollGroundState::Update()
{
	//	カウンター更新
	counter_.Update();

	//	上から下にポッドを落とす
	Vector3D pos = sPod_->GetPosition();
	pos.y = EaseIn(startPosY_, endPosY_, counter_.GetCountPerMaxCount(), Quint);
	sPod_->SetPosition(pos);

	//	カメラのターゲット更新
	camera->SetTarget(pos + targetToPod_);
	//	シェイク
	const float noShakeV = 0.0f;
	float shakeValue = EaseIn(noShakeV, maxShakeV_, counter_.GetCountPerMaxCount(), Double);
	camera->SetShake(-shakeValue, shakeValue);

	//	カウントが終わったら次のステートへ
	if (counter_.GetFrameCount() == moveMaxFrame_) {
		std::unique_ptr<EscPodState> next_ = std::make_unique<PodCollGroundState>();
		sPod_->SetNextState(next_);
	}
}
