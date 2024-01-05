#include "UIPosition.h"

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

Vector2D& UIPosition::GetPosition()
{
	return pos_;
}

Vector2D& UIPosition::GetSize()
{
	return size_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void UIPosition::SetPosition(const Vector2D& pos)
{
	pos_ = pos;
}

void UIPosition::SetSize(const Vector2D& size)
{
	size_ = size;
}
