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

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

int32_t UIAnimationTimer::GetStartCount()
{
	return startCount_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void UIAnimationTimer::SetStartCount(int32_t startCount)
{
	startCount_ = startCount;
}

void UIAnimationTimer::Reset()
{
	FrameCounter::SetIsIncrement(parent_->GetIsStartAnimation());
	
	FrameCounter::ResetCount();
}
