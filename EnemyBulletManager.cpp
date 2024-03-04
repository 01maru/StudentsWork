#include "EnemyBulletManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "CollisionManager.h"
#include "EnemyBullet.h"
#include "CollisionAttribute.h"

using namespace MNE;
using namespace CollAttribute;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void EnemyBulletManager::Initialize()
{
	attribute_ = COLLISION_ATTR_LANDSHAPE | COLLISION_ATTR_ALLIES;
}

void EnemyBulletManager::LoadResources()
{
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void EnemyBulletManager::Update(std::list<EnemyBulletInfo>& bullets)
{
	//	弾生成
	for (auto& itr : bullets)
	{
		std::unique_ptr<IBullet> bullet = std::make_unique<EnemyBullet>();
		bullet->Initialize();
		bullet->SetLifeTime(itr.lifeTime_);
		bullet->SetSpd(itr.spd_);
		bullet->SetMoveVec(itr.moveVec_);
		bullet->SetModel(ModelManager::GetInstance()->GetModel("bullet"));
		bullet->SetPosition(itr.pos_);

		bullet->SetAttribute(COLLISION_ATTR_ENEMY_AT);

		bullets_.push_back(std::move(bullet));
	}
	bullets.clear();

	IBulletManager::Update();
}
