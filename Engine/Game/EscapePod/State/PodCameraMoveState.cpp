#include "PodCameraMoveState.h"
#include "EscapePod.h"
#include "CameraManager.h"
#include "Easing.h"
#include "UIData.h"

using namespace Easing;
using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PodCameraMoveState::Initialize()
{
	//	開始位置設定
	camera = CameraManager::GetInstance()->GetMainCamera();
	startEye_ = camera->GetEye();
	startTarget_ = camera->GetTarget();

	//	カウントスタート
	counter_.Initialize(cameraMoveFrame_, TRUE);
	counter_.StartCount();

	//	ムービー用黒帯非表示設定(アニメーションして消える)
	sPod_->GetLetterBoxPtr()->ResetAnimation(FALSE);

	//	アニメーションをドアの空いた後の動きに変更
	sPod_->GetAnimation()->SetAnimeName("AfterOpen");
	sPod_->GetAnimation()->SetAutoPlay(TRUE);		//	自動で再生
	sPod_->GetAnimation()->SetAnimeTimer(0.0f);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PodCameraMoveState::Update()
{
	//	カウント更新
	counter_.Update();

	//	カメラの移動処理
	camera->SetEye(EaseIn(startEye_, endEye_, counter_.GetCountPerMaxCount(), Single));
	camera->SetTarget(EaseIn(startTarget_, endTarget_, counter_.GetCountPerMaxCount(), Single));

	//	カウントが終了したら
	if (counter_.GetFrameCount() == cameraMoveFrame_) {
		std::unique_ptr<EscPodState> next_ = nullptr;
		sPod_->SetNextState(next_);
		//	ポッドの演出終了
		sPod_->SetOpenDoor(TRUE);
	}
}
