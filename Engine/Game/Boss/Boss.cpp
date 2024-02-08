#include "Boss.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "BossStartState.h"

#include "Player.h"
#include "Quaternion.h"

#include "ImGuiManager.h"

#include "IGameState.h"

using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void Boss::StatusInitialize()
{
	hp_.SetMaxHP(maxHP_);

	//	初期ステート
	std::unique_ptr<BossState> next = std::make_unique<BossStartState>();
	SetCurrentState(next);

	hp_.Initialize();
}

void Boss::Initialize(MNE::IModel* model)
{
	Object3D::Initialize();
	SetModel(model);
	BossState::SetBoss(this);

	StatusInitialize();

	float radius = 3.0f;
	MyMath::Vector3D offset;
	SetCollider(new SphereCollider(offset, radius));
	collider_->SetAttribute(CollAttribute::COLLISION_ATTR_ENEMYS);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

float Boss::RotationUpdate()
{
	frontVec_ = mat_.trans_;
	frontVec_ -= player_->GetCenterPos();
	Vector2D vec2(frontVec_.x, frontVec_.z);
	float dis = frontVec_.GetLength();
	mat_.angle_.x = atan2(-frontVec_.y, vec2.GetLength());
	frontVec_.Normalize();
	mat_.angle_.y = atan2(frontVec_.x, frontVec_.z);

	return dis;
}

void Boss::Update()
{
	if (isActive_ == FALSE) return;

	//	死亡判定
	hp_.Update();

	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->GetIsActive() == false;
		});

	currentState_->Update();

	for (auto itr = bullets_.begin(); itr != bullets_.end(); itr++)
	{
		itr->get()->Update();
	}

	if (hp_.GetIsAlive() == true) {

		ColliderUpdate();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Collision
//-----------------------------------------------------------------------------

void Boss::CollisionUpdate()
{
	MatUpdate();
	collider_->Update();
}

void Boss::OnCollision(CollisionInfo& info)
{
	(void)info;
}

//-----------------------------------------------------------------------------
// [SECTION] ImGuiUpdate
//-----------------------------------------------------------------------------

void Boss::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->BeginWindow("PlayerStatus", true);

	imgui->Text("frontVec : (%.2f, %.2f, %.2f)", frontVec_.x, frontVec_.y, frontVec_.z);

	imgui->EndWindow();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void Boss::DrawBullets()
{
	for (auto& itr : bullets_)
	{
		itr->Draw();
	}
}

void Boss::DrawUI()
{
	hp_.Draw();
}

void Boss::AddBullet(std::unique_ptr<EnemyBullet>& bullet)
{
	bullets_.push_back(std::move(bullet));
}

void Boss::DecHP(int32_t damage)
{
	hp_.DecHp(damage);
}

void Boss::StartClearState()
{
	pClearState_->Start();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool Boss::GetIsAlive()
{
	return hp_.GetIsAlive();
}

bool Boss::GetIsHPLessThanHalf()
{
	int32_t halfHP = hp_.GetMaxHP() / 2;
	return hp_.GetHP() <= halfHP;
}

bool Boss::GetIsSecondForm()
{
	return isSecondForm_;
}

bool Boss::GetBodyAttack()
{
	return bodyAt_;
}

MyMath::Vector3D* Boss::GetPositionPtr()
{
	return &mat_.trans_;
}

MyMath::Vector3D Boss::GetFrontVec()
{
	return frontVec_;
}

Player* Boss::GetPlayerPtr()
{
	return player_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void Boss::SetIsSecondForm(bool isSecondForm)
{
	isSecondForm_ = isSecondForm;
}

void Boss::SetIsActive(bool isActive)
{
	isActive_ = isActive;
}

void Boss::SetCurrentState(std::unique_ptr<BossState>& next)
{
	currentState_ = std::move(next);
	currentState_->Initialize();
}

void Boss::SetPlayer(Player* player)
{
	player_ = player;
}

void Boss::SetHPBarSprite(const MNE::Sprite& sprite)
{
	hp_.SetSprite(sprite);

	MyMath::Vector3D red(1.0f, 0.0f, 0.0f);
	hp_.SetBarColor(red);
}

void Boss::SetBodyAttack(bool attackFlag)
{
	bodyAt_ = attackFlag;
}

void Boss::SetClearState(IGameState* clearState)
{
	pClearState_ = clearState;
}
