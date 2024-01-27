#include "GameOverUI.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "XAudioManager.h"
#include "CameraManager.h"
#include "GameOverCamera.h"
#include "Easing.h"

using namespace Easing;
using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void GameOverUI::Initialize()
{
	//	カーソルの初期化
	cursor_.Initialize();
	//	初期位置設定(音再生しない)
	cursor_.SetCursorPosition(data_.GetSelectPosition(), false);
}

void GameOverUI::LoadResources()
{
	//	配置データ
	data_.LoadData("GameOver");
	//	カーソル
	cursor_.LoadResources();
	//	選択音
	XAudioManager::GetInstance()->LoadSoundWave("decision.wav");
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void GameOverUI::InputUpdate()
{
	bool dikButton = InputManager::GetInstance()->GetPad()->GetButtonTrigger(InputJoypad::A_Button);

	data_.CollisonCursorUpdate();

	//	選択されたら
	if ((dikButton || data_.GetSelect()) == TRUE)
	{
		//	決定音再生
		XAudioManager::GetInstance()->PlaySoundWave("decision.wav", XAudioManager::SE);

		if (data_.GetSelectName() == "Continue") {
			//	リセット
			isActive_ = false;
		}

		if (data_.GetSelectName() == "Quit") {
			//	タイトルに戻る
			SceneManager::GetInstance()->SetNextScene("TITLESCENE");
		}
	}

	InputJoypad* pad = InputManager::GetInstance()->GetPad();

	int16_t inputValue = 0;

	if (pad->GetTriggerThumbLY() == TRUE)
	{
		inputValue = static_cast<int16_t>(MyMath::mClamp(-1.0f, 1.0f, -pad->GetThumbL().y));
	}
	//	選択中のボタン切り替え
	data_.InputUpdate(inputValue);
	//	カーソル移動
	cursor_.SetCursorPosition(data_.GetSelectPosition());
	//	カーソルのアニメーションサイズ設定
	cursor_.SetButtonSize(data_.GetSelectSize());
}

void GameOverUI::Update()
{
	//	実行中じゃなかったら処理しない
	if (isActive_ == FALSE) return;

	//	カメラのズームが終了していなかったら処理しない
	if (pCamera_->GetEndCameraMove() == FALSE) return;

	if (animeDirtyFlag_ == TRUE) {
		animeDirtyFlag_ = FALSE;
		data_.ResetAnimation(true);
	}
	InputManager::GetInstance()->SetDrawExplane(data_.GetIsEndAnimation());

	InputUpdate();

	data_.Update();

	//	タイトルアニメーション中じゃないときにカーソル表示
	cursor_.SetIsActive(data_.GetIsEndAnimation());

	//	カーソル更新
	cursor_.Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void GameOverUI::Draw()
{
	//	実行中じゃなかったら描画しない
	if (isActive_ == FALSE) return;

	data_.Draw();

	//	カーソル描画
	cursor_.Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void GameOverUI::SetCameraPosData(const Vector3D& playerPos)
{
	pCamera_->SetPosData(playerPos);
}

void GameOverUI::Start()
{
	isActive_ = true;

	InputManager::GetInstance()->GetMouse()->SetLockCursor(false);
	//	カメラ変更
	CameraManager* cameraMan = CameraManager::GetInstance();
	std::unique_ptr<ICamera> camera = std::make_unique<GameOverCamera>();
	camera->Initialize(cameraMan->GetMainCamera()->GetEye()
		, cameraMan->GetMainCamera()->GetTarget()
		, cameraMan->GetMainCamera()->GetUp());
	cameraMan->SetMainCamera(camera);

	pCamera_ = dynamic_cast<GameOverCamera*>(cameraMan->GetMainCamera());
}

void GameOverUI::Reset()
{
	if (pCamera_ == nullptr) return;

	isActive_ = true;
	pCamera_->Reset();
	data_.Reset();
	cursor_.SetIsActive(false);
	animeDirtyFlag_ = TRUE;
}
