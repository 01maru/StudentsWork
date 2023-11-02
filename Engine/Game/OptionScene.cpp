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

void OptionScene::SensUpdate()
{
	InputManager* input = InputManager::GetInstance();

	UIObject* obj = data_.GetUIObject("Sens");
	
	float sens = input->GetSensitivity();
	
	//	値変更&反映
	int32_t inputValue = input->GetKeyAndButton(DIK_D, InputJoypad::DPAD_Right) -
		input->GetKeyAndButton(DIK_A, InputJoypad::DPAD_Left);
	SliderSprite* slider = obj->GetComponent<SliderSprite>();
	slider->ValueUpdate(sens, inputValue);
	input->SetSensitivity(sens);
}

void OptionScene::MasterVolumeUpdate()
{
}

void OptionScene::BGMVolumeUpdate()
{
}

void OptionScene::SEVolumeUpdate()
{
}

void OptionScene::InputUpdate(bool selectButton)
{
	if (!isActive_) return;

	data_.InputUpdate();

	if (data_.GetSelectName() == "Sens") {
		SensUpdate();
	}

	//else if (data_.GetSelectName() == "Volume") {
	//	MasterVolumeUpdate();
	//}

	//else if (data_.GetSelectName() == "Volume") {
	//	BGMVolumeUpdate();
	//}

	//else if (data_.GetSelectName() == "Volume") {
	//	SEVolumeUpdate();
	//}

	else if (data_.GetSelectName() == "Back") {
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
	data_.SetSelectButton("Sens");
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool OptionScene::GetIsActive()
{
	return isActive_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void OptionScene::SetIsActive(bool active)
{
	isActive_ = active;
}
