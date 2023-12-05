#include "OptionScene.h"
#include "InputManager.h"
#include "ImGuiManager.h"
#include "SelectCursor.h"

void OptionScene::Initialize()
{
	data_.Initialize();

	//	円の位置初期化
	SliderSprite* slider = data_.GetUIObject("Sens")->GetComponent<SliderSprite>();
	slider->SetValue(InputManager::GetInstance()->GetSensitivity());
}

void OptionScene::LoadResources(const std::string& filename)
{
	data_.LoadData(filename);

#pragma region Sound

	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("decision.wav");

#pragma endregion
}

void OptionScene::SensUpdate(int16_t inputValue)
{
	InputManager* input = InputManager::GetInstance();

	UIObject* obj = data_.GetUIObject("Sens");
	
	float sens = input->GetSensitivity();
	
	//	値変更&反映
	SliderSprite* slider = obj->GetComponent<SliderSprite>();
	slider->ValueUpdate(sens, inputValue);
	input->SetSensitivity(sens);
}

void OptionScene::VolumeUpdate(const std::string& objectName, XAudioManager::SoundType type, int16_t inputValue)
{
	UIObject* obj = data_.GetUIObject(objectName);

	XAudioManager* xAudio = XAudioManager::GetInstance();
	float volume;
	switch (type)
	{
	case XAudioManager::Master:
		volume = xAudio->GetMasterVolume();
		break;
	case XAudioManager::BGM:
		volume = xAudio->GetBGMVolume();
		break;
	case XAudioManager::SE:
		volume = xAudio->GetSEVolume();
		break;
	}

	//	値変更&反映
	SliderSprite* slider = obj->GetComponent<SliderSprite>();
	slider->ValueUpdate(volume, inputValue);

	xAudio->VolumeUpdate(type, volume);
}

void OptionScene::InputUpdate(bool selectButton)
{
	if (!isActive_) return;

	InputManager* input = InputManager::GetInstance();
	int16_t inputValue = input->GetKeyAndButton(DIK_D, InputJoypad::DPAD_Right) -
		input->GetKeyAndButton(DIK_A, InputJoypad::DPAD_Left);

	data_.InputUpdate();

	if (data_.GetSelectName() == "Sens") {
		SensUpdate(inputValue);
	}

	else if (data_.GetSelectName() == "Master") {
		VolumeUpdate("Master", XAudioManager::Master, inputValue);
	}

	else if (data_.GetSelectName() == "BGM") {
		VolumeUpdate("BGM", XAudioManager::BGM, inputValue);
	}

	else if (data_.GetSelectName() == "SE") {
		VolumeUpdate("SE", XAudioManager::SE, inputValue);
	}

	else if (data_.GetSelectName() == "Quit") {
		if (selectButton == true) {
			isActive_ = false;
			//	決定音再生
			XAudioManager::GetInstance()->PlaySoundWave("decision.wav", XAudioManager::SE);
			
			cursor_->SetCursorPosition(backPos_, false);
		}
	}
}

void OptionScene::Update()
{
	data_.Update();

	//	オプション中だったら
	if (isActive_ == false) return;

	//	カーソル移動
	cursor_->SetCursorPosition(data_.GetSelectPosition());
	cursor_->SetMinSize(GetSelectScale());
	cursor_->SetMaxSize(GetSelectScale() + Vector2D(16, 8));
}

void OptionScene::ImGuiUpdate()
{
	if (!isActive_) return;

	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->Text("SelectButton : %s", data_.GetSelectName().c_str());
}

void OptionScene::Draw()
{
	if (!isActive_) return;

	data_.Draw();
}

void OptionScene::ResetSelectButton()
{
	data_.SetSelectButton("Master");
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool OptionScene::GetIsActive()
{
	return isActive_;
}

Vector2D& OptionScene::GetSelectPosition()
{
	return data_.GetSelectPosition();
}

Vector2D OptionScene::GetSelectScale()
{
	Vector2D ans(818, 82);
	Vector2D quitButtonSize(298, 82);
	if (data_.GetSelectName() == "Quit") {
		ans = quitButtonSize;
	}
	return ans;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void OptionScene::SetCursorBackPos(const Vector2D& pos)
{
	backPos_ = pos;
}

void OptionScene::SetSelectCursor(SelectCursor* cursor)
{
	cursor_ = cursor;
}

void OptionScene::SetIsActive(bool active)
{
	isActive_ = active;
}
