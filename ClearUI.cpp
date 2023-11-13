#include "ClearUI.h"
#include "SceneManager.h"
#include "InputManager.h"

void ClearUI::Initialize()
{
	data_.LoadData("Clear");
}

void ClearUI::Update()
{
	if (isActive_ == false) return;
	bool select = InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_BACKSPACE, InputJoypad::A_Button);

	if (select)
	{
		isActive_ = false;
		//XAudioManager::GetInstance()->PlaySoundWave("decision.wav", XAudioManager::SE);

		//if (data_.GetSelectName() == "Continue") {
		//}

		//if (data_.GetSelectName() == "Quit") {
			//SceneManager::GetInstance()->SetNextScene("TITLESCENE");
		//}
	}

	data_.Update();
}

void ClearUI::Draw()
{
	if (isActive_ == false) return;
	data_.Draw();
}

void ClearUI::Start()
{
	isActive_ = true;
	data_.Initialize();
}
