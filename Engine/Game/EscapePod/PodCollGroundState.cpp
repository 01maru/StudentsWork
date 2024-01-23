#include "PodCollGroundState.h"
#include "EscapePod.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "Easing.h"
#include "PodBefOpenDoorState.h"

using namespace Easing;
using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PodCollGroundState::Initialize()
{
	//	カウンター初期化
	counter_.Initialize(collGroundShakeFrame_, true);
	counter_.StartCount();

	//	メインカメラ設定
	camera = CameraManager::GetInstance()->GetMainCamera();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PodCollGroundState::Update()
{
	//	カウンター更新
	counter_.Update();

	//	カメラの揺れ
	float shakeValue = EaseOut(maxShakeV_, 0.0f, counter_.GetCountPerMaxCount(), Double);
	camera->SetShake(-shakeValue, shakeValue);
	
	//	時間になったら
	if (counter_.GetFrameCount() == collGroundShakeFrame_) {
		//	カメラの揺れを止める
		camera->StopShake();

		//	ドアを開けるUIの表示
		sPod_->StartUICounter();

		bool dikButton = InputManager::GetInstance()->GetKeyAndButton(DIK_E, InputJoypad::X_Button);
		
		//	ボタンが押されたらドアを開ける
		if (dikButton == TRUE)
		{
			sPod_->SetDrawUI(false);
			//	次のStateへ
			std::unique_ptr<EscPodState> next_ = std::make_unique<PodBefOpenDoorState>();
			sPod_->SetNextState(next_);
		}
	}
}
