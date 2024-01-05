#include "UIFadeAnimation.h"
#include "UIObject.h"
#include "Easing.h"
#include "UIAnimationTimer.h"
#include "UISprite.h"

using namespace Easing;

void UIFadeAnimation::Initialize()
{
	sprite_ = parent_->GetComponent<UISprite>();
	timer_ = parent_->GetComponent<UIAnimationTimer>();
}

void UIFadeAnimation::Update()
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

float UIFadeAnimation::GetStart()
{
	return start_;
}

float UIFadeAnimation::GetEnd()
{
	return end_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void UIFadeAnimation::SetStart(float start)
{
	start_ = start;
}

void UIFadeAnimation::SetEnd(float end)
{
	end_ = end;
}

void UIFadeAnimation::Reset()
{
	for (auto& sprite : sprite_->GetSprites())
	{
		sprite.second.SetAlphaColor(start_);
	}
}
