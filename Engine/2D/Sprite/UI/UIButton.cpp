#include "UIButton.h"
#include "UIObject.h"
#include "UIPosition.h"
#include "Vector2D.h"

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

Vector2D& UIButton::GetPosition()
{
	return parent_->GetComponent<UIPosition>()->GetPosition();
}

Vector2D& UIButton::GetSize()
{
	return parent_->GetComponent<UIPosition>()->GetSize();
}

const std::string& UIButton::GetButtonName()
{
	return name_;
}

UIButton* UIButton::GetNextButton()
{
	return pNext_;
}

UIButton* UIButton::GetPrevButton()
{
	return pPrev_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void UIButton::SetName(const std::string& name)
{
	name_ = name;
}

void UIButton::SetNextButton(UIButton* pNext)
{
	pNext_ = pNext;
}

void UIButton::SetPrevButton(UIButton* pPrev)
{
	pPrev_ = pPrev;
}
