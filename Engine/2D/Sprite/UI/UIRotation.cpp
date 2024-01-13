#include "UIRotation.h"
#include "UIObject.h"
#include "UISprite.h"

void MNE::UIRotation::Update()
{
    if (pSprites_ == nullptr) {
        pSprites_ = parent_->GetComponent<UISprite>();
    }

    if (pSprites_ == nullptr) return;
    
    for (auto& sprite : pSprites_->GetSprites())
    {
        float rot = sprite.second.GetRotation();
        rot += spd_;
        sprite.second.SetRotation(rot);
    }
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

float MNE::UIRotation::GetRotSpd()
{
    return spd_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::UIRotation::SetRotSpd(float spd)
{
    spd_ = spd;
}
