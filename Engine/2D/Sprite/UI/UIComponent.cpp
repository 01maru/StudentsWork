#include "UIComponent.h"
#include "UIObject.h"

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::UIComponent::SetParent(UIObject* parent)
{
	parent_ = parent;
}
