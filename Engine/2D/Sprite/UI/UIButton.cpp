#include "UIButton.h"
#include "UIObject.h"
#include "UIPosition.h"
#include "Vector2D.h"

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

MyMath::Vector2D& MNE::UIButton::GetPosition()
{
	return parent_->GetComponent<UIPosition>()->GetPosition();
}

MyMath::Vector2D& MNE::UIButton::GetSize()
{
	return parent_->GetComponent<UIPosition>()->GetSize();
}

const std::string& MNE::UIButton::GetButtonName()
{
	return name_;
}

MNE::UIButton* MNE::UIButton::GetNextButton()
{
	return pNext_;
}

MNE::UIButton* MNE::UIButton::GetPrevButton()
{
	return pPrev_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::UIButton::SetName(const std::string& name)
{
	name_ = name;
}

void MNE::UIButton::SetNextButton(UIButton* pNext)
{
	pNext_ = pNext;
}

void MNE::UIButton::SetPrevButton(UIButton* pPrev)
{
	pPrev_ = pPrev;
}
