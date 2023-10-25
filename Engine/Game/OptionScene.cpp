//#include "OptionScene.h"
//#include "InputManager.h"
//
//void OptionScene::Initialize(const std::string& filename)
//{
//	LoadSprites(filename);
//}
//
//void OptionScene::Update()
//{
//	if (!active_) return;
//
//	InputManager* input = InputManager::GetInstance();
//
//	int16_t change = input->GetTriggerKeyAndButton(DIK_S, InputJoypad::DPAD_Down) - input->GetTriggerKeyAndButton(DIK_W, InputJoypad::DPAD_Up);
//
//	mord_ += change;
//	mord_ = (uint16_t)MyMath::mLoop(Close, Sens, (int16_t)mord_);
//
//	//UIData::Update();
//}
//
//void OptionScene::Draw()
//{
//	if (!active_) return;
//
//	UIDrawer::Draw();
//}
