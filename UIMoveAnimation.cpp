#include "UIMoveAnimation.h"
#include "UIObject.h"
#include "Easing.h"
#include "UIAnimationTimer.h"
#include "UISprite.h"

using namespace Easing;

void UIMoveAnimation::Initialize()
{
	sprite_ = parent_->GetComponent<UISprite>();
	timer_ = parent_->GetComponent<UIAnimationTimer>();
}

void UIMoveAnimation::Update()
{
	if (sprite_ == nullptr) return;

	for (auto& sprite : sprite_->GetSprites())
	{
		Vector2D ans = EaseIn(startPos, endPos, timer_->GetCountPerMaxCount(), 5);
		
		sprite.second.SetPosition(ans);
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

Vector2D& UIMoveAnimation::GetStartPos()
{
	return startPos;
}

Vector2D& UIMoveAnimation::GetEndPos()
{
	return endPos;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void UIMoveAnimation::SetStartPos(const Vector2D& pos)
{
	startPos = pos;
}

void UIMoveAnimation::SetEndPos(const Vector2D& pos)
{
	endPos = pos;
}
