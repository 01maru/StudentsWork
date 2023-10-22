#include "UIAnimationTimer.h"
#include "UIObject.h"

void UIAnimationTimer::Initialize()
{
	FrameCounter::Initialize(60, true);
}

void UIAnimationTimer::Update()
{
	if (parent_->GetCount()->GetFrameCount() == startCount_&& parent_->GetCount()->GetIsActive()) {
		FrameCounter::StartCount();
	}
	FrameCounter::Update();
}
