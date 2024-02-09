#include "PauseScreen.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "XAudioManager.h"
#include "ImGuiManager.h"
#include "CameraManager.h"
#include "GameCamera.h"
#include "GameScene.h"

using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PauseScreen::Initialize()
{
	//	初期値は非アクティブ状態
	isActive_ = FALSE;
	
	//	リソース読み込み
	LoadResources();

	//	配置データ
	pauseData_.ResetAnimation(TRUE);

	//	カーソルの初期化
	cursor_.Initialize();
	//	初期位置設定(音再生しない)
	cursor_.SetCursorPosition(pauseData_.GetSelectPosition(), FALSE);

	//	オプションの初期化
	option_.Initialize();
	//	カーソル設定
	option_.SetSelectCursor(&cursor_);
}

void PauseScreen::LoadResources()
{
#pragma region Sound

	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("decision.wav");

#pragma endregion

	//	ポーズの配置データ読み込み
	pauseData_.LoadData("Pause");
	//	オプションの配置データ読み込み
	option_.LoadResources("Option");
	//	カーソルリソース読み込み
	cursor_.LoadResources();
}

void PauseScreen::MouseCursorInit()
{
	//	ポーズ画面になったら
	bool lockCursor = isActive_ == FALSE;

	//	カーソル表示
	InputManager::GetInstance()->GetMouse()->SetLockCursor(lockCursor);
}

void PauseScreen::InputValueUpdate()
{
	InputJoypad* pad = InputManager::GetInstance()->GetPad();

	inputValue_ = 0;

	if (pad->GetTriggerThumbLY() == TRUE)
	{
		inputValue_ = static_cast<int16_t>(MyMath::mClamp(-inputSpd_, inputSpd_, -pad->GetThumbL().y));
	}
}

void PauseScreen::PauseInitialize(bool isActive)
{
	isActive_ = isActive;

	//	ステートの設定
	if (isActive_ == TRUE)
	{
		prevState_ = gameScene_->GetNowState();
		gameScene_->SetNextState(GameScene::PauseState);
	}
	else
	{
		gameScene_->SetNextState(prevState_);
		//	ポーズアニメーション中じゃないときにカーソル表示
		cursor_.SetIsActive(FALSE);
	}

	//	カメラの動き停止
	gameCamera_->SetIsActive(!isActive_);
	//	マウスのロック解除
	MouseCursorInit();

	//	ポーズ画面出現or消す
	pauseData_.ResetAnimation(isActive_);

	//	選択中のボタンを初期化する
	pauseData_.SetSelectButton("Resume");

	InputManager::GetInstance()->SetNextTag("cantBack", TRUE, isActive_);
	InputManager::GetInstance()->SetDrawExplane(isActive_);

}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

bool PauseScreen::IsActiveUpdate()
{
	//	オプション中だったら更新しない
	if (option_.GetIsActive() == TRUE) return FALSE;

	InputManager* input = InputManager::GetInstance();
	bool dikButton = input->GetTriggerKeyAndButton(DIK_ESCAPE, InputJoypad::START_Button);

	//	キー入力されたら
	if (dikButton) {
		PauseInitialize(!isActive_);
		
		return TRUE;
	}

	return FALSE;
}

void PauseScreen::PauseInputUpdate(bool dikSelectButton)
{
	//	オプション中だったら更新しない
	if (option_.GetIsActive() == TRUE) return;

	pauseData_.CollisonCursorUpdate();

	//	選択ボタン入力中だったら
	if ((dikSelectButton || pauseData_.GetSelectMouse()) == TRUE) {
		//	決定音再生
		XAudioManager::GetInstance()->PlaySoundWave("decision.wav", XAudioManager::SE);

		const std::string buttonName = pauseData_.GetSelectName();

		//	ゲームに戻る
		if (buttonName == "Resume") {
			PauseInitialize(FALSE);
		}

		//	オプション画面を開く
		else if (buttonName == "Option") {
			//	オプション画面へ
			option_.SetIsActive(TRUE);
			//	選択中のボタンを初期化する
			option_.ResetSelectButton();
			//	変更前のカーソルの位置保存
			option_.SetCursorBackPos(pauseData_.GetSelectPosition());
			//	カーソルの位置変更(音再生しない)
			cursor_.SetCursorPosition(option_.GetSelectPosition(), FALSE);
			//	オプション出現
			option_.ResetAnimation(TRUE);
			//	ポーズ画面消す
			pauseData_.ResetAnimation(FALSE);

			InputManager::GetInstance()->SetNextTag("canBack", TRUE, FALSE);
		}

		//	タイトルに戻る
		else if (buttonName == "Quit") {
			SceneManager::GetInstance()->SetNextScene("TITLESCENE");
		}
	}

	//	ポーズの選択ボタン切り替え
	pauseData_.InputUpdate(inputValue_);

	//	カーソル移動
	cursor_.SetCursorPosition(pauseData_.GetSelectPosition());
	//	カーソルのアニメーションサイズ設定
	cursor_.SetButtonSize(pauseData_.GetSelectSize());
}

void PauseScreen::PauseUpdate(bool dikSelectButton)
{
	//	ポーズ入力処理
	PauseInputUpdate(dikSelectButton);

	//	オプション中じゃなかったら
	if (option_.GetIsActive() == FALSE)
	{
		//	ポーズアニメーション中じゃないときにカーソル表示
		cursor_.SetIsActive(pauseData_.GetIsEndAnimation());
	}
}

void PauseScreen::OptionUpdate(bool dikSelectButton)
{
	//	オプション入力処理(オプションが終了したタイミングだったら)
	if (option_.InputUpdate(dikSelectButton, inputValue_) == TRUE)
	{
		//	ポーズ出現
		pauseData_.ResetAnimation(TRUE);

		//	ポーズアニメーション中じゃないときにカーソル表示
		cursor_.SetIsActive(pauseData_.GetIsEndAnimation());

		InputManager::GetInstance()->SetNextTag("cantBack", TRUE, FALSE);
	}
}

void PauseScreen::Update()
{
	//	ポーズ中じゃなく、アニメーション中じゃなかったら更新しない
	if (isActive_ == FALSE && pauseData_.GetIsEndAnimation() == TRUE) return;

	bool dikButton = InputManager::GetInstance()->GetPad()->GetButtonTrigger(InputJoypad::A_Button);

	InputValueUpdate();

	//	ポーズの更新処理
	PauseUpdate(dikButton);

	//	オプションの更新処理
	OptionUpdate(dikButton);
}

void PauseScreen::AnimationUpdate()
{
	//	ポーズ中じゃなく、アニメーション中じゃなかったら更新しない
	if (isActive_ == FALSE && pauseData_.GetIsEndAnimation() == TRUE) return;

	//	ポーズ更新処理
	pauseData_.Update();

	//	オプションデータの更新処理
	option_.Update();

	//	カーソル更新
	cursor_.Update();
}

void PauseScreen::ImGuiUpdate()
{
	ImGuiManager* imGui = ImGuiManager::GetInstance();

	//	ポーズ中か確認用
	imGui->Text("Pause : %s", isActive_ ? "True" : "False");
	//	選択しているボタン名表示
	imGui->Text("SelectButton : %s", pauseData_.GetSelectName().c_str());

	//	オプションのImGui更新
	option_.ImGuiUpdate();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void PauseScreen::Draw()
{
	//	ポーズ中じゃなく、アニメーション中じゃなかったら描画しない
	if (isActive_ == FALSE && pauseData_.GetIsEndAnimation() == TRUE) return;

	//	ポーズ描画
    pauseData_.Draw();

	//	オプション描画
	option_.Draw();

	//	カーソル描画
	if ((pauseData_.GetSelecting() && option_.GetIsActive() == FALSE) ||
		(option_.GetIsActive() && option_.GetSelecting()))
	{
		cursor_.Draw();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool PauseScreen::GetIsActive()
{
	return isActive_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void PauseScreen::SetIsActive(bool isActive)
{
    isActive_ = isActive;
}

void PauseScreen::SetGameCamera(GameCamera* gameCamera)
{
	gameCamera_ = gameCamera;
}

void PauseScreen::SetGameScene(GameScene* gameScene)
{
	gameScene_ = gameScene;
}
