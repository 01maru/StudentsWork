#include "UIObject.h"
#include "FrameCounter.h"

void UIObject::Update()
{
	for (auto itr = components_.begin(); itr != components_.end(); ++itr)
	{
		itr->get()->Update();
	}
}

void UIObject::MatUpdate()
{
	for (auto itr = components_.begin(); itr != components_.end(); ++itr)
	{
		itr->get()->MatUpdate();
	}
}

void UIObject::Draw()
{
	for (auto itr = components_.begin(); itr != components_.end(); ++itr)
	{
		itr->get()->Draw();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

FrameCounter* UIObject::GetCount()
{
	return count_;
}

bool UIObject::GetIsStartAnimation()
{
	return *startAnimation_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void UIObject::SetCount(FrameCounter* pCount)
{
	count_ = pCount;
}

void UIObject::SetStartAnimation(bool* pStartAnimation)
{
	startAnimation_ = pStartAnimation;
}

void UIObject::ResetAnimation()
{
	for (auto& comp : components_)
	{
		comp->Reset();
	}
}
