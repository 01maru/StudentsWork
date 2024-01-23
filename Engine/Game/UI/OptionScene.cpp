#include "OptionScene.h"
#include "SelectCursor.h"
#include "SliderSprite.h"
#include "XAudioManager.h"
#include "InputManager.h"
#include "ImGuiManager.h"

using namespace MNE;

void OptionScene::Initialize()
{
	//	円の位置初期化
	//	感度のスライダー値初期化
	SliderSprite* slider = data_.GetUIObject("Sens")->GetComponent<SliderSprite>();
	slider->SetValue(InputManager::GetInstance()->GetSensitivity());

	//	音のスライダー値初期化
	XAudioManager* xAudio = XAudioManager::GetInstance();
	//	マスターボリューム
	slider = data_.GetUIObject("Master")->GetComponent<SliderSprite>();
	slider->SetValue(xAudio->GetMasterVolume());
	//	マスターボリューム
	slider = data_.GetUIObject("BGM")->GetComponent<SliderSprite>();
	slider->SetValue(xAudio->GetBGMVolume());
	//	マスターボリューム
	slider = data_.GetUIObject("SE")->GetComponent<SliderSprite>();
	slider->SetValue(xAudio->GetSEVolume());
}

void OptionScene::LoadResources(const std::string& filename)
{
	//	配置データ読み込み
	data_.LoadData(filename);

#pragma region Sound

	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("decision.wav");

#pragma endregion
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void OptionScene::SensUpdate(int16_t inputValue)
{
	InputManager* input = InputManager::GetInstance();

	//	感度オブジェクト取得
	UIObject* obj = data_.GetUIObject("Sens");
	
	//	現在の感度値取得
	float sens = input->GetSensitivity();
	
	//	値変更
	SliderSprite* slider = obj->GetComponent<SliderSprite>();
	slider->ValueUpdate(sens, inputValue);
	//	値反映
	input->SetSensitivity(sens);
}

void OptionScene::VolumeUpdate(const std::string& objectName, int16_t inputValue)
{
	//	指定のオブジェクトスライダー取得
	SliderSprite* slider = data_.GetUIObject(objectName)->GetComponent<SliderSprite>();

	XAudioManager* xAudio = XAudioManager::GetInstance();
	float volume;
	XAudioManager::SoundType type = XAudioManager::None;
	
	//	マスターボリュームだったら
	if (objectName == "Master") {
		volume = xAudio->GetMasterVolume();
		type = XAudioManager::Master;
	}

	//	BGMボリュームだったら
	else if (objectName == "BGM") {
		volume = xAudio->GetBGMVolume();
		type = XAudioManager::BGM;
	}

	//	SEボリュームだったら
	else if (objectName == "SE") {
		volume = xAudio->GetSEVolume();
		type = XAudioManager::SE;
	}

	//	値変更
	slider->ValueUpdate(volume, inputValue);
	//	値反映
	xAudio->VolumeUpdate(type, volume);
}

void OptionScene::CloseOption()
{
	//	非アクティブに
	isActive_ = false;
	//	決定音再生
	XAudioManager::GetInstance()->PlaySoundWave("decision.wav", XAudioManager::SE);
	//	カーソルをオプション前の位置へ(音再生しない)
	cursor_->SetCursorPosition(befCursorPos_, false);
	//	オプション閉じる
	data_.ResetAnimation(false);
}

bool OptionScene::InputUpdate(bool dikSelectButton, int16_t inputV)
{
	//	非アクティブだったら更新終了
	if (isActive_ == FALSE) return false;

	InputManager* input = InputManager::GetInstance();

	if (input->GetPad()->GetButtonTrigger(InputJoypad::B_Button))
	{
		CloseOption();

		return true;
	}

	//	スライダー用左右入力値取得
	int16_t inputValue = input->GetKeyAndButton(DIK_D, InputJoypad::DPAD_Right) -
		input->GetKeyAndButton(DIK_A, InputJoypad::DPAD_Left);

	//	選択中ボタンの更新
	data_.InputUpdate(inputV);

	//	選択位置が感度だったら
	if (data_.GetSelectName() == "Sens") {
		SensUpdate(inputValue);
	}

	//	閉じるだったら
	else if (data_.GetSelectName() == "Quit") {

		//	選択中だったら
		if (dikSelectButton == TRUE) {
			CloseOption();

			return true;
		}
	}

	//	その他(音量調節)だったら
	else {
		VolumeUpdate(data_.GetSelectName(), inputValue);
	}

	return false;
}

void OptionScene::Update()
{
	//	配置データの更新
	data_.Update();

	//	オプション中だったら更新終了
	if (isActive_ == FALSE) return;

	//	カーソル移動
	cursor_->SetCursorPosition(data_.GetSelectPosition());
	cursor_->SetButtonSize(data_.GetSelectSize());

	//	アニメーション中じゃないときにカーソル表示
	cursor_->SetIsActive(data_.GetIsEndAnimation());
}

void OptionScene::ImGuiUpdate()
{
	//	オプション中だったら更新終了
	if (isActive_ == FALSE) return;

	ImGuiManager* imgui = ImGuiManager::GetInstance();

	//	選択中のボタンの名前
	imgui->Text("SelectButton : %s", data_.GetSelectName().c_str());
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void OptionScene::Draw()
{
	//	アクティブ状態じゃなくアニメーション中じゃなかったら描画しない
	if (isActive_ == FALSE && data_.GetIsEndAnimation() == TRUE) return;

	data_.Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool OptionScene::GetIsActive()
{
	return isActive_;
}

Vector2D OptionScene::GetSelectPosition()
{
	return data_.GetSelectPosition();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void OptionScene::SetIsActive(bool active)
{
	isActive_ = active;
}

void OptionScene::SetCursorBackPos(const Vector2D& pos)
{
	befCursorPos_ = pos;
}

void OptionScene::SetSelectCursor(SelectCursor* cursor)
{
	cursor_ = cursor;
}

//-----------------------------------------------------------------------------
// [SECTION] Reset
//-----------------------------------------------------------------------------

void OptionScene::ResetSelectButton()
{
	data_.SetSelectButton("Master");
}

void OptionScene::ResetAnimation(bool startingAnimation)
{
	data_.ResetAnimation(startingAnimation);
}
