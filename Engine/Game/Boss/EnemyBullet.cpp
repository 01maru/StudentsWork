#include "EnemyBullet.h"
#include "CollisionAttribute.h"
#include "BaseCollider.h"
#include "SphereCollider.h"
#include "Player.h"

using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Destructor
//-----------------------------------------------------------------------------

EnemyBullet::~EnemyBullet()
{
}

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void EnemyBullet::Initialize()
{
	IBullet::Initialize();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void EnemyBullet::Update()
{
	IBullet::Update();

	MatUpdate();
	ColliderUpdate();
}

void EnemyBullet::OnCollision(CollisionInfo& info)
{
	(void)info;
	Player* player = nullptr;

	switch (info.GetCollider()->GetAttribute())
	{
	case CollAttribute::COLLISION_ATTR_LANDSHAPE:
		//lifeTime_.SetIsActive(false);
		//	ヒット演出
		break;
	case CollAttribute::COLLISION_ATTR_ALLIES:
		player = dynamic_cast<Player*>(info.GetCollider()->GetObject3D());
		player->DecHP(damage_);
		lifeTime_.SetIsActive(false);
		break;
	default:
		break;
	}
}
