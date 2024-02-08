#include "UIScaling.h"
#include "UIObject.h"
#include "UISprite.h"
#include "UIAnimationTimer.h"
#include "Easing.h"

using namespace Easing;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void MNE::UIScaling::Initialize()
{
    pSprites_ = parent_->GetComponent<UISprite>();
    pTimer_ = parent_->GetComponent<UIAnimationTimer>();
}

void MNE::UIScaling::Update()
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

MyMath::Vector2D& MNE::UIScaling::GetStartSize()
{
    return startSize_;
}

MyMath::Vector2D& MNE::UIScaling::GetEndSize()
{
    return endSize_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::UIScaling::SetStartSize(const MyMath::Vector2D& size)
{
    startSize_ = size;
}

void MNE::UIScaling::SetEndSize(const MyMath::Vector2D& size)
{
    endSize_ = size;
}
