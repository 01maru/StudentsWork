#include "UIButton.h"

void UIButton::Update()
{
	//	“–‚½‚è”»’è
}

void UIButton::SetNumber(int16_t& index)
{
	if (!isActive_)	return;

	number_ = index;
	index++;
}
