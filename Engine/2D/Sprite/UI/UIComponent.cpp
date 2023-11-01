#include "UIComponent.h"
#include "UIObject.h"

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void UIComponent::SetParent(UIObject* parent)
{
	parent_ = parent;
}
