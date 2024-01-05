#include "UIButtonManager.h"
#include "UIButton.h"
#include "InputManager.h"

void UIButtonManager::Update()
{
	InputManager* inputMan = InputManager::GetInstance();
	
	if (inputMan->GetTriggerKeyAndButton(DIK_W, InputJoypad::DPAD_Up)) {
		selectButton_ = selectButton_->GetPrevButton();
	}
	
	if (inputMan->GetTriggerKeyAndButton(DIK_S, InputJoypad::DPAD_Down)) {
		selectButton_ = selectButton_->GetNextButton();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

const std::string& UIButtonManager::GetSelectObjName()
{
	return selectButton_->GetButtonName();
}

Vector2D& UIButtonManager::GetSelectPos()
{
	return selectButton_->GetPosition();
}

Vector2D& UIButtonManager::GetSelectSize()
{
	return selectButton_->GetSize();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void UIButtonManager::SetSelectButton(UIButton* button)
{
	selectButton_ = button;
}
