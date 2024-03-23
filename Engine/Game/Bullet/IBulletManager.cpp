#include "IBulletManager.h"
#include "ImGuiManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

using namespace MNE;
using namespace CollAttribute;

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void IBulletManager::Update()
{
	//	弾削除
	bullets_.remove_if([](std::unique_ptr<IBullet>& bullet) {
		return bullet->GetIsActive() == false;
		});

	//	弾更新
	for (auto& itr : bullets_)
	{
		itr->Update();
	}
}

void IBulletManager::CollisionUpdate()
{
	for (auto& bullet : bullets_)
	{
		CollisionManager::GetInstance()->CheckCollision(bullet->GetCollider(), attribute_);
	}
}

void IBulletManager::ImGuiUpdate()
{
	ImGuiManager* imGui = ImGuiManager::GetInstance();

	imGui->Text("bullet : %d", bullets_.size());
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void IBulletManager::Draw()
{
	for (auto& itr : bullets_)
	{
		itr->Draw();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void IBulletManager::SetAttribute(uint16_t attribute)
{
	attribute_ = attribute;
}
