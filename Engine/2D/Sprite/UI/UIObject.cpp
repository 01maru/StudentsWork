#include "UIObject.h"
#include "FrameCounter.h"

void MNE::UIObject::Update()
{
	for (auto itr = components_.begin(); itr != components_.end(); ++itr)
	{
		itr->get()->Update();
	}
}

void MNE::UIObject::MatUpdate()
{
	for (auto itr = components_.begin(); itr != components_.end(); ++itr)
	{
		itr->get()->MatUpdate();
	}
}

void MNE::UIObject::Draw()
{
	for (auto itr = components_.begin(); itr != components_.end(); ++itr)
	{
		itr->get()->Draw();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

FrameCounter* MNE::UIObject::GetCount()
{
	return count_;
}

bool MNE::UIObject::GetIsStartAnimation()
{
	return *startAnimation_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::UIObject::SetCount(FrameCounter* pCount)
{
	count_ = pCount;
}

void MNE::UIObject::SetStartAnimation(bool* pStartAnimation)
{
	startAnimation_ = pStartAnimation;
}

void MNE::UIObject::ResetAnimation()
{
	for (auto& comp : components_)
	{
		comp->Reset();
	}
}
