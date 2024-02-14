#include "BaseCollider.h"
#include <cassert>

//-----------------------------------------------------------------------------
// [SECTION] Finalize
//-----------------------------------------------------------------------------

void MNE::BaseCollider::Finalize()
{
    deleteFlag_ = TRUE;
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

MyMath::Matrix MNE::BaseCollider::GetWorldMatrix()
{
    //  ワールド行列が設定されていなかったら
    if (matWorld_ == nullptr)
    {
        //  オブジェクトが設定されていなかったら停止
        assert(!(object_ == nullptr));

        //  オブジェクトの行列を返す
        return object_->GetMatWorld();
    }

    return *matWorld_;
}

bool MNE::BaseCollider::GetDeleteFlag()
{
    return deleteFlag_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------
