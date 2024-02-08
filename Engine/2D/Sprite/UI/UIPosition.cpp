#include "UIPosition.h"

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

MyMath::Vector2D& MNE::UIPosition::GetPosition()
{
	return pos_;
}

MyMath::Vector2D& MNE::UIPosition::GetSize()
{
	return size_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::UIPosition::SetPosition(const MyMath::Vector2D& pos)
{
	pos_ = pos;
}

void MNE::UIPosition::SetSize(const MyMath::Vector2D& size)
{
	size_ = size;
}
