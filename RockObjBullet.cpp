#include "RockObjBullet.h"
#include "CollisionManager.h"
#include "RayCast.h"
#include "CollisionAttribute.h"
#include "Player.h"

using namespace MNE;
using namespace MNE::CollAttribute;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void RockObjBullet::Initialize()
{
	IBullet::Initialize();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void RockObjBullet::Update()
{
	//	移動
	mat_.trans_ += moveVec_ * spd_;

	OnGroundUpdate();

	MatUpdate();
	ColliderUpdate();
}

void RockObjBullet::OnCollision(MNE::CollisionInfo& info)
{
	Player* player = nullptr;

	switch (info.GetCollider()->GetAttribute())
	{
	case COLLISION_ATTR_ALLIES:

		player = dynamic_cast<Player*>(info.GetCollider()->GetObject3D());
		player->DecHP(damage_);

		//	岩に押しつぶされている場合
		if (player->GetPosition().y <= mat_.trans_.y)
		{
			lifeTime_.SetIsActive(FALSE);
		}
		//	岩が移動中に触れた場合
		else
		{

		}
		break;
	default:
		break;
	}
}

void RockObjBullet::OnGroundUpdate()
{
	////	着地済みだったら処理しない
	//if (onGround_ == TRUE) return;

	//Ray ray;
	//ray.start = mat_.trans_;
	//ray.start.y += sphereCollider->GetRadius();
	//Vector3D downVec(0, -1, 0);
	//ray.dir = downVec;
	//RayCast rayCastHit;

	////	着地
	//if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &rayCastHit,
	//	diameter)) {
	//	onGround_ = TRUE;
	//	mat_.trans_.y -= (rayCastHit.distance - diameter);
	//	Object3D::ColliderUpdate();
	//	//	オブジェクトに変更
	//	collider_->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	//	
	//}
}
