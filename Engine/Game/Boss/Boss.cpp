#include "Boss.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "BossStartState.h"

#include "Player.h"
#include "Quaternion.h"

#include "ImGuiManager.h"

#include "IGameState.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

#include "BossBeamState.h"

using namespace MNE;
using namespace MyMath;
using namespace CollAttribute;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void Boss::StatusInitialize()
{
	hp_.SetMaxHP(maxHP_);

	//	初期ステート
	std::unique_ptr<BossState> next = std::make_unique<BossBeamState>();
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
	MyMath::Vector3D offset(0.0f, 3.0f, 0.0f);
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

	currentState_->SetStateForSpecificSituation();

	currentState_->Update();

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
	ImGuiManager* imGui = ImGuiManager::GetInstance();

	imGui->BeginWindow("PlayerStatus", true);

	imGui->Text("frontVec : (%.2f, %.2f, %.2f)", frontVec_.x, frontVec_.y, frontVec_.z);

	imGui->EndWindow();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void Boss::DrawUI()
{
	hp_.Draw();
}

void Boss::DrawBeam()
{
	currentState_->Draw();
}

void Boss::AddBullet(EnemyBulletInfo& bullet)
{
	bullets_.push_back(bullet);
}

void Boss::DecHP(int32_t damage)
{
	hp_.DecHp(damage);
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

MyMath::Vector3D Boss::GetShotPoint()
{
	Vector3D offset(0.0f, 3.0f, 0.0f);
	return mat_.trans_ + offset;
}

MyMath::Vector3D Boss::GetBeamPoint()
{
	Vector3D offset(0.0f, 3.0f, 0.0f);
	return mat_.trans_ + offset;
}

std::list<EnemyBulletInfo>& Boss::GetBullets()
{
	return bullets_;
}

GameScene* Boss::GetGameScene()
{
	return pGameScene_;
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

void Boss::SetGameScene(GameScene* gameScene)
{
	pGameScene_ = gameScene;
}
