#include "UIFadeAnimation.h"
#include "UIObject.h"
#include "Easing.h"
#include "UIAnimationTimer.h"
#include "UISprite.h"

using namespace Easing;

void MNE::UIFadeAnimation::Initialize()
{
	sprite_ = parent_->GetComponent<UISprite>();
	timer_ = parent_->GetComponent<UIAnimationTimer>();
}

void MNE::UIFadeAnimation::Update()
{
	if (sprite_ == nullptr) return;

	for (auto& sprite : sprite_->GetSprites())
	{
		float fade = EaseIn(start_, end_, timer_->GetCountPerMaxCount(), 5);

		sprite.second.SetAlphaColor(fade);
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

float MNE::UIFadeAnimation::GetStart()
{
	return start_;
}

float MNE::UIFadeAnimation::GetEnd()
{
	return end_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::UIFadeAnimation::SetStart(float start)
{
	start_ = start;
}

void MNE::UIFadeAnimation::SetEnd(float end)
{
	end_ = end;
}

void MNE::UIFadeAnimation::Reset()
{
	for (auto& sprite : sprite_->GetSprites())
	{
		sprite.second.SetAlphaColor(start_);
	}
}
