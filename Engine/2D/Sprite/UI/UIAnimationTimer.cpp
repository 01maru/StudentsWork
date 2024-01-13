#include "UIAnimationTimer.h"
#include "UIObject.h"

void MNE::UIAnimationTimer::Initialize()
{
	FrameCounter::Initialize(60, true);
}

void MNE::UIAnimationTimer::Update()
{
	if (parent_->GetCount()->GetFrameCount() == startCount_&& parent_->GetCount()->GetIsActive()) {
		FrameCounter::StartCount();
	}
	FrameCounter::Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

int32_t MNE::UIAnimationTimer::GetStartCount()
{
	return startCount_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::UIAnimationTimer::SetStartCount(int32_t startCount)
{
	startCount_ = startCount;
}

void MNE::UIAnimationTimer::Reset()
{
	FrameCounter::SetIsIncrement(parent_->GetIsStartAnimation());
	
	FrameCounter::ResetCount();
}
