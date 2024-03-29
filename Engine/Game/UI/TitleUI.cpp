#include "TitleUI.h"
#include "InputManager.h"
#include "XAudioManager.h"
#include "SceneManager.h"
#include "ImGuiManager.h"
#include "TitleCamera.h"

using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void TitleUI::LoadResources()
{
#pragma region Sound

	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("decision.wav");

#pragma endregion

	//	配置データ
	titleData_.LoadData("Title");
	//	カーソル
	cursor_.LoadResources();
	//	オプション
	option_.LoadResources("Option");
}

void TitleUI::Initialize()
{
	//	リソース読み込み
	LoadResources();

	//	カーソルの初期化
	cursor_.Initialize();
	//	初期位置設定(音再生しない)
	cursor_.SetCursorPosition(titleData_.GetSelectPosition(), false);

	//	オプションの初期化
	option_.Initialize();
	//	カーソル設定
	option_.SetSelectCursor(&cursor_);

	InputManager* input = InputManager::GetInstance();

	//	初期表示設定
	input->SetNextTag("cantBack", input->GetUsePad(), input->GetUsePad());
	//	入力説明表示
	input->SetDrawExplane(true);

	//	カーソル固定解除
	input->GetMouse()->SetLockCursor(false);
}

void TitleUI::Finalize()
{
	//	操作説明用スプライト非表示
	InputManager::GetInstance()->SetDrawExplane(FALSE);
}

//-----------------------------------------------------------------------------
// [SECTION] TitleUpdate
//-----------------------------------------------------------------------------

void TitleUI::InputValueUpdate()
{
	InputJoypad* pad = InputManager::GetInstance()->GetPad();

	//	リセット
	inputValue_ = 0;

	if (pad->GetTriggerThumbLY() == TRUE)
	{
		inputValue_ = static_cast<int16_t>(MyMath::mClamp(-inputSpd_, inputSpd_, -pad->GetThumbL().y));
	}
}

void TitleUI::TitleInputUpdate(bool dikSelectButton)
{
	//	オプションシーンだったら処理しない
	if (option_.GetIsActive() == TRUE)				return;

	//	演出が終わっていなかったら処理しない
	if (titleData_.GetIsEndAnimation() == FALSE)	return;

	titleData_.CollisonCursorUpdate();

	//	ボタンを選択したら
	if ((dikSelectButton || titleData_.GetSelectMouse()) == TRUE)
	{
		//	決定音再生
		XAudioManager::GetInstance()->PlaySoundWave("decision.wav", XAudioManager::SE);

		const std::string selectButtonName = titleData_.GetSelectName();

		//	Start選択中だったら
		if (selectButtonName == "Start")
		{
			//	タイトル消える
			titleData_.ResetAnimation(FALSE);
			//	カメラ動かす
			pCamera_->SetNextMode(TitleCamera::SceneChange);

			//	ゲームシーンへ
			SceneManager::GetInstance()->SetNextScene("GAMESCENE");
		}

		//	Option選択中だったら
		else if (selectButtonName == "Settings") {
			//	オプション画面へ
			option_.SetIsActive(TRUE);
			//	選択中のボタンを初期化する
			option_.ResetSelectButton();
			//	変更前のカーソルの位置保存
			option_.SetCursorBackPos(titleData_.GetSelectPosition());
			//	カーソルの位置変更(音再生しない)
			cursor_.SetCursorPosition(option_.GetSelectPosition(), FALSE);

			//	タイトル消える
			titleData_.ResetAnimation(FALSE);
			//	オプション出現
			option_.ResetAnimation(TRUE);

			//	カメラ動かす
			pCamera_->SetNextMode(TitleCamera::Option);

			InputManager::GetInstance()->SetNextTag("canBack", TRUE, FALSE);
		}

		//	Quit選択中だったら
		else if (selectButtonName == "Quit") {
			//	ゲーム終了
			SceneManager::GetInstance()->GameLoopEnd();
		}
	}

	//	タイトルの選択ボタン切り替え
	titleData_.InputUpdate(inputValue_);

	//	カーソル移動
	cursor_.SetCursorPosition(titleData_.GetSelectPosition());
	//	カーソルのアニメーションサイズ設定
	cursor_.SetButtonSize(titleData_.GetSelectSize());
}

void TitleUI::TitleUpdate(bool dikSelectButton)
{
	//	タイトル入力処理
	TitleInputUpdate(dikSelectButton);

	//	オプション中じゃなかったら
	if (option_.GetIsActive() == FALSE)
	{
		//	タイトルアニメーション中じゃないときにカーソル表示
		cursor_.SetIsActive(titleData_.GetIsEndAnimation());
	}

	//	タイトル更新処理
	titleData_.Update();
}

//-----------------------------------------------------------------------------
// [SECTION] OptionUpdate
//-----------------------------------------------------------------------------

void TitleUI::OptionUpdate(bool dikSelectButton)
{
	//	オプション入力処理(オプションが終了したタイミングだったら)
	if (option_.InputUpdate(dikSelectButton, inputValue_) == TRUE)
	{
		//	タイトル出現
		titleData_.ResetAnimation(TRUE);

		//	タイトルアニメーション中じゃないときにカーソル表示
		cursor_.SetIsActive(titleData_.GetIsEndAnimation());

		//	カメラ戻す
		pCamera_->SetNextMode(TitleCamera::Menu);

		InputManager::GetInstance()->SetNextTag("cantBack", TRUE, FALSE);
	}

	//	オプションデータの更新処理
	option_.Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void TitleUI::Update()
{
	bool dikButton = InputManager::GetInstance()->GetPad()->GetButtonTrigger(InputJoypad::A_Button);

	InputValueUpdate();

	//	タイトル更新
	TitleUpdate(dikButton);

	//	オプション更新
	OptionUpdate(dikButton);

	//	カーソル更新
	cursor_.Update();
}

void TitleUI::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	//	選択中のモード
	imgui->Text("mode : %s", titleData_.GetSelectName().c_str());

	//	オプションImGui
	option_.ImGuiUpdate();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void TitleUI::Draw()
{
	//	タイトル描画
	titleData_.Draw();

	//	オプション描画
	option_.Draw();
	
	//	カーソル描画
	if ((titleData_.GetSelecting() && option_.GetIsActive() == FALSE) ||
		(option_.GetIsActive() && option_.GetSelecting()))
	{
		cursor_.Draw();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void TitleUI::Start()
{
	titleData_.ResetAnimation(TRUE);
}

void TitleUI::SetTitleCamera(TitleCamera* pCamera)
{
	pCamera_ = pCamera;
}
