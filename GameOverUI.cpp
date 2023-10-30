#include "GameOverUI.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "XAudioManager.h"

GameOverUI* GameOverUI::GetInstance()
{
	static GameOverUI instance;
	return &instance;
}

void GameOverUI::Initialize()
{
	data_.LoadData("GameOver");
}

void GameOverUI::Update()
{
	if (isActive_ == false) return;
//	bool select = InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button);
//
//	if (select)
//	{
//		XAudioManager::GetInstance()->PlaySoundWave("decision.wav", XAudioManager::SE);
//
//		//if (data_.GetSelectName() == "Continue") {
//		//}
//
//		if (data_.GetSelectName() == "Quit") {
//			SceneManager::GetInstance()->SetNextScene("TITLESCENE");
//		}
//	}

	data_.Update();
}

void GameOverUI::Draw()
{
	if (isActive_ == false) return;
	data_.Draw();
}

void GameOverUI::Start()
{
	isActive_ = true;
	data_.Initialize();
}
