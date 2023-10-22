#include "UIComponent.h"
#include "UIObject.h"

void UIComponent::SetParent(UIObject* parent)
{
	parent_ = parent;
}
