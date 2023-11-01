#include "OptionScene.h"
#include "InputManager.h"

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
	
	//	値変更


	//	値反映
	SliderSprite* slider = obj->GetComponent<SliderSprite>();
	slider->SetValue(sens);
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

void OptionScene::Update()
{
	if (!isActive_) return;

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

	data_.Update();
}

void OptionScene::Draw()
{
	if (!isActive_) return;

	data_.Draw();
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
