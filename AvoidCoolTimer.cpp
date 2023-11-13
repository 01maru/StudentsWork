#include "AvoidCoolTimer.h"
#include "InputManager.h"
#include "ImGuiManager.h"

void AvoidCoolTimer::Update()
{
	CoolTime::Update();

	InputUpdate();
}

void AvoidCoolTimer::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->Text("AvoidCoolTimer : %d", coolTimer_.GetFrameCount() );
}

void AvoidCoolTimer::InputUpdate()
{
	//	クールタイム中だったら
	if (isActive_ == false) return;

	InputManager* input = InputManager::GetInstance();
	if (input->GetTriggerKeyAndButton(DIK_LSHIFT, InputJoypad::B_Button)) {
		isAvoiding_ = true;
		StartCount();
	}
}

bool AvoidCoolTimer::GetIsAvoiding()
{
	return isAvoiding_;
}

void AvoidCoolTimer::SetIsAvoiding(bool isAvoiding)
{
	isAvoiding_ = isAvoiding;
}
