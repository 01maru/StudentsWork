#include "EnemyBulletManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "CollisionManager.h"
#include "EnemyBullet.h"
#include "EnemyBumpStone.h"
#include "CollisionAttribute.h"

using namespace MNE;
using namespace MNE::CollAttribute;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void EnemyBulletManager::Initialize()
{
	attribute_ = COLLISION_ATTR_LANDSHAPE | COLLISION_ATTR_ALLIES;
	beamObj_.Initialize();
	beamObj_.SetModel(ModelManager::GetInstance()->GetModel("Beam"));
}

void EnemyBulletManager::LoadResources()
{
	ModelManager::GetInstance()->LoadModel("Beam");
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void EnemyBulletManager::Update(std::list<EnemyBulletInfo>& bullets, const BeamInfo& beam)
{
	//	弾生成
	for (auto& itr : bullets)
	{
		std::unique_ptr<IBullet> bullet;

		if (itr.type_ == NormalStone)
		{
			bullet = std::make_unique<EnemyBullet>();
			bullet->Initialize();
			bullet->SetLifeTime(itr.lifeTime_);
			bullet->SetSpd(itr.spd_);
			bullet->SetMoveVec(itr.moveVec_);
			bullet->SetModel(ModelManager::GetInstance()->GetModel("bullet"));
			bullet->SetPosition(itr.pos_);

			bullet->SetAttribute(COLLISION_ATTR_ENEMY_AT);
		}
		else if (itr.type_ == BumpStone)
		{
			std::unique_ptr<EnemyBumpStone> bump = std::make_unique<EnemyBumpStone>();
			bump->Initialize();
			bump->SetLifeTime(itr.lifeTime_);
			bump->SetSpd(itr.spd_);
			bump->SetMoveVec(itr.moveVec_);
			bump->SetModel(ModelManager::GetInstance()->GetModel("Beam"));
			bump->SetPosition(itr.pos_);
			bump->SetScale(itr.scale_);

			bump->SetAttribute(COLLISION_ATTR_ENEMY_AT);

			bullet = std::move(bump);
		}

		bullets_.push_back(std::move(bullet));
	}
	bullets.clear();

	beamActive_ = beam.active_;
	if (beam.active_ == TRUE)
	{
		beamObj_.SetPosition(beam.pos_);
		beamObj_.SetScale(beam.scale_);
		beamObj_.SetRotMatrix(beam.rotMat_);
		beamObj_.SetColor(beam.color_);
		beamObj_.MatUpdate(FALSE);
	}

	//	弾削除
	for (auto& itr : bullets_)
	{
		if (itr->GetIsActive() == FALSE)
		{
			if (itr->GetCollider()->GetAttribute() & COLLISION_ATTR_LANDSHAPE)
			{
				rocks_.emplace_back(itr);
			}

			bullets_.remove(itr);
		}
	}

	IBulletManager::Update();
}

void EnemyBulletManager::Draw()
{
	if (beamActive_ == TRUE)
	{
		beamObj_.Draw();
	}

	IBulletManager::Draw();
}
