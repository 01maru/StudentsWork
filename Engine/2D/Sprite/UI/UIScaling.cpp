#include "UIScaling.h"
#include "UIObject.h"
#include "UISprite.h"
#include "UIAnimationTimer.h"
#include "Easing.h"

using namespace Easing;

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void UIScaling::Initialize()
{
    pSprites_ = parent_->GetComponent<UISprite>();
    pTimer_ = parent_->GetComponent<UIAnimationTimer>();
}

void UIScaling::Update()
{
    if (pSprites_ == nullptr) return;

    for (auto& sprite : pSprites_->GetSprites())
    {
        Vector2D size = EaseOut(startSize_, endSize_, pTimer_->GetCountPerMaxCount(), 3);
        sprite.second.SetSize(size);
    }
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

Vector2D& UIScaling::GetStartSize()
{
    return startSize_;
}

Vector2D& UIScaling::GetEndSize()
{
    return endSize_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void UIScaling::SetStartSize(const Vector2D& size)
{
    startSize_ = size;
}

void UIScaling::SetEndSize(const Vector2D& size)
{
    endSize_ = size;
}
