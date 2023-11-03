#include "OptionScene.h"
#include "InputManager.h"
#include "ImGuiManager.h"

void OptionScene::Initialize(const std::string& filename)
{
	data_.LoadData(filename);
	data_.Initialize();

	//	円の位置初期化
	SliderSprite* slider = data_.GetUIObject("Sens")->GetComponent<SliderSprite>();
	slider->SetValue(InputManager::GetInstance()->GetSensitivity());
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
		}
	}
}

void OptionScene::Update()
{
	data_.Update();
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

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void OptionScene::SetIsActive(bool active)
{
	isActive_ = active;
}
