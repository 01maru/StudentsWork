#include "UIPosition.h"

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

Vector2D& MNE::UIPosition::GetPosition()
{
	return pos_;
}

Vector2D& MNE::UIPosition::GetSize()
{
	return size_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::UIPosition::SetPosition(const Vector2D& pos)
{
	pos_ = pos;
}

void MNE::UIPosition::SetSize(const Vector2D& size)
{
	size_ = size;
}
