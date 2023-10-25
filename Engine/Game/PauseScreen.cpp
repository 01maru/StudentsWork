//#include "PauseScreen.h"
//#include "InputManager.h"
//#include "SceneManager.h"
//
//void PauseScreen::Initialize()
//{
//	active_ = false;
//
//    selectMord_ = Continue;
//
//	ui_ = std::make_unique<UIDrawer>();
//	ui_->LoadSprites("Pause");
//
//	option_ = std::make_unique<OptionScene>();
//	option_->Initialize("Option");
//}
//
//void PauseScreen::PauseUpdate()
//{
//	//if (InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button)) {
//	//	switch (selectMord_)
//	//	{
//	//	case Continue:
//	//		active_ = false;
//	//		break;
//	//	case Option:
//	//		option_->SetIsActive(true);
//	//		ui_->SetTag(UIData::Tag3);
//	//		break;
//	//	case BackTitle:
//	//		SceneManager::GetInstance()->SetNextScene("TITLESCENE");
//	//		break;
//	//	}
//	//}
//}
//
//void PauseScreen::Update()
//{
//	if (InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_ESCAPE, InputJoypad::START_Button)) {
//		active_ = !active_;
//		if (active_) InputManager::GetInstance()->GetMouse()->SetLockCursor(false);
//	}
//
//    if (!active_) return;
//
//	if (option_->GetIsActive())	option_->Update();
//	else						PauseUpdate();
//
//    //ui_->Update();
//}
//
//void PauseScreen::Draw()
//{
//    if (!active_) return;
//
//    ui_->Draw();
//
//	option_->Draw();
//}
//
//void PauseScreen::SetIsActive(bool active)
//{
//    if (active) InputManager::GetInstance()->GetMouse()->SetLockCursor(false);
//    active_ = active;
//}
