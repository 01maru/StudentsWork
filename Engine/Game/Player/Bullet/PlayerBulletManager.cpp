#include "PlayerBulletManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

using namespace MNE;
using namespace CollAttribute;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void PlayerBulletManager::LoadResources()
{
	ModelManager* models = ModelManager::GetInstance();
	models->LoadModel("bullet");
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void PlayerBulletManager::Update(std::list<BulletInfo>& bullets)
{
	//	弾生成
	for (auto& itr : bullets)
	{
		std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
		bullet->Initialize();
		bullet->SetLifeTime(itr.lifeTime_);
		bullet->SetSpd(itr.spd_);
		bullet->SetMoveVec(itr.moveVec_);
		bullet->SetModel(ModelManager::GetInstance()->GetModel("bullet"));
		bullet->SetPosition(itr.pos_);

		bullets_.push_back(std::move(bullet));
	}
	bullets.clear();

	//	弾削除
	bullets_.remove_if([](std::unique_ptr<Bullet>& bullet) {
		return bullet->GetIsActive() == false;
		});

	//	弾更新
	for (auto& itr : bullets_)
	{
		itr->Update();
	}
}

void PlayerBulletManager::CollisionUpdate()
{
	for (auto& bullet : bullets_)
	{
		CollisionManager::GetInstance()->CheckCollision(bullet->GetCollider(), COLLISION_ATTR_LANDSHAPE | COLLISION_ATTR_ENEMYS);
	}
}

void PlayerBulletManager::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->Text("bullet : %d", bullets_.size());
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void PlayerBulletManager::Draw()
{
	for (auto& itr : bullets_)
	{
		itr->Draw();
	}
}
