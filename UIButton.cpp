#include "UIButton.h"

void UIButton::Update()
{
	//	�����蔻��
}

void UIButton::SetNumber(int16_t& index)
{
	if (!isActive_)	return;

	number_ = index;
	index++;
}
