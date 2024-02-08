#include "PodOpenDoorState.h"
#include "EscapePod.h"
#include "CameraManager.h"
#include "Easing.h"
#include "PodCameraMoveState.h"

using namespace Easing;
using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PodOpenDoorState::Initialize()
{
	//	カメラ初期位置設定
	camera = CameraManager::GetInstance()->GetMainCamera();
	startEye_ = camera->GetEye();
	startTarget_ = camera->GetTarget();

	//	カウントスタート
	counter_.Initialize(openDoorFrame_, TRUE);
	counter_.StartCount();

	//	移動量計算
	move_ = startTarget_ - startEye_;
	//	上下の移動はさせない
	const float groundHeight = 0.0f;
	move_.y = groundHeight;

	//	ドアを開けるアニメーションに変更
	sPod_->GetAnimation()->SetAnimeName("Open");
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PodOpenDoorState::Update()
{
	//	カウンター更新
	counter_.Update();

	//	アニメーション更新
	sPod_->GetAnimation()->SetAnimeTimer(static_cast<float>(counter_.GetFrameCount()));

	//	視点移動
	Vector3D move = EaseIn(Vector3D(), move_, counter_.GetCountPerMaxCount(), Double);
	Vector3D eye = startEye_ + move;
	camera->SetEye(eye);
	//	ターゲット移動
	Vector3D target = startTarget_ + move;
	camera->SetTarget(target);

	//	カウント終わったら次のステートへ
	if (counter_.GetIsActive() == FALSE) {
		sPod_->SetDrawPlayer(TRUE);
		std::unique_ptr<EscPodState> next_ = std::make_unique<PodCameraMoveState>();
		sPod_->SetNextState(next_);
	}
}
