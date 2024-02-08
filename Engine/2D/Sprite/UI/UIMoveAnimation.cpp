#include "UIMoveAnimation.h"
#include "UIObject.h"
#include "Easing.h"
#include "UIAnimationTimer.h"
#include "UISprite.h"

using namespace Easing;
using namespace MyMath;

void MNE::UIMoveAnimation::Initialize()
{
	sprite_ = parent_->GetComponent<UISprite>();
	timer_ = parent_->GetComponent<UIAnimationTimer>();
}

void MNE::UIMoveAnimation::Update()
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

Vector2D& MNE::UIMoveAnimation::GetStartPos()
{
	return startPos;
}

Vector2D& MNE::UIMoveAnimation::GetEndPos()
{
	return endPos;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::UIMoveAnimation::SetStartPos(const Vector2D& pos)
{
	startPos = pos;
}

void MNE::UIMoveAnimation::SetEndPos(const Vector2D& pos)
{
	endPos = pos;
}

void MNE::UIMoveAnimation::Reset()
{
	for (auto& sprite : sprite_->GetSprites())
	{
		sprite.second.SetPosition(startPos);
	}
}
