#include "UIButtonManager.h"
#include "UIButton.h"
#include "InputManager.h"

using namespace MyMath;

void MNE::UIButtonManager::Update(int16_t inputValue)
{
	int16_t inputV = inputValue;

	while (inputV != 0)
	{
		if (inputV > 0)
		{
			selectButton_ = selectButton_->GetNextButton();
			inputV--;
		}
		else
		{
			selectButton_ = selectButton_->GetPrevButton();
			inputV++;
		}
	}
	//if (inputMan->GetTriggerKeyAndButton(DIK_W, InputJoypad::DPAD_Up)) {
	//	selectButton_ = selectButton_->GetPrevButton();
	//}
	//
	//if (inputMan->GetTriggerKeyAndButton(DIK_S, InputJoypad::DPAD_Down)) {
	//	selectButton_ = selectButton_->GetNextButton();
	//}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

const std::string& MNE::UIButtonManager::GetSelectObjName()
{
	return selectButton_->GetButtonName();
}

MyMath::Vector2D& MNE::UIButtonManager::GetSelectPos()
{
	return selectButton_->GetPosition();
}

MyMath::Vector2D& MNE::UIButtonManager::GetSelectSize()
{
	return selectButton_->GetSize();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::UIButtonManager::SetSelectButton(UIButton* button)
{
	selectButton_ = button;
}
