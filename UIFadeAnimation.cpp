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

		sprite.second.SetColor(Vector4D(1.0f, 1.0f, 1.0f, fade));
	}
}
