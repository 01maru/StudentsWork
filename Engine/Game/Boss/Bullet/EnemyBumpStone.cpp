#include "EnemyBumpStone.h"
#include "Player.h"
#include "CollisionAttribute.h"
#include "BaseCollider.h"

using namespace MNE::CollAttribute;
using namespace MNE;

EnemyBumpStone::~EnemyBumpStone()
{
}

void EnemyBumpStone::Initialize()
{
	IBullet::Initialize();
}

void EnemyBumpStone::Update()
{
	lifeTime_.Update();

	mat_.scale_.y = height_ * lifeTime_.GetCountPerMaxCount();

	MatUpdate();
	ColliderUpdate();
}

void EnemyBumpStone::OnCollision(MNE::CollisionInfo& info)
{
	Player* player = nullptr;

	switch (info.GetCollider()->GetAttribute())
	{
	case COLLISION_ATTR_ALLIES:

		player = dynamic_cast<Player*>(info.GetCollider()->GetObject3D());
		player->DecHP(damage_);

		lifeTime_.SetIsActive(FALSE);

		break;
	default:
		break;
	}
}

void EnemyBumpStone::SetScale(const MyMath::Vector3D& scale)
{
	mat_.scale_ = scale;
	height_ = scale.y;
}
