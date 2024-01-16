#include "Boss.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "BossIdleState.h"

#include "Player.h"
#include "Quaternion.h"

#include "ImGuiManager.h"

#include "IGameState.h"

using namespace MNE;

void Boss::StatusInitialize()
{
	hp_.SetMaxHP(maxHP_);

	//	初期ステート
	currentState_ = std::make_unique<BossIdleState>();
	EnemyState::SetBoss(this);

	Vector3D pos(0.0f, 3.0f, 10.0f);
	mat_.trans_ = pos;

	hp_.Initialize();
}

void Boss::Initialize(MNE::IModel* model)
{
	Object3D::Initialize();
	SetModel(model);

	StatusInitialize();

	float radius = 3.0f;
	Vector3D offset;
	SetCollider(new SphereCollider(offset, radius));
	collider_->SetAttribute(CollAttribute::COLLISION_ATTR_ENEMYS);
}

void Boss::Update()
{
	if (isActive_ == FALSE) return;

	//	死亡判定
	hp_.Update();

	//	生きていたら


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

void Boss::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->BeginWindow("PlayerStatus", true);

	imgui->Text("frontVec : (%.2f, %.2f, %.2f)", frontVec_.x, frontVec_.y, frontVec_.z);

	imgui->EndWindow();
}

void Boss::DrawBullets()
{
	for (auto itr = bullets_.begin(); itr != bullets_.end(); itr++)
	{
		itr->get()->Draw();
	}
}

void Boss::DrawUI()
{
	hp_.Draw();
}

void Boss::CollisionUpdate()
{
	MatUpdate();
	collider_->Update();
}

float Boss::RotationUpdate()
{
	//Vector3D dir = player_->GetPosition() - mat_.trans_;
	//dir.Normalize();
	//frontVec_.Normalize();
	//Quaternion q = DirectionToDirection(frontVec_, dir);
	//mat_.matRot_ *= q.GetRotMatrix();
	//frontVec_ = dir;

	frontVec_ = mat_.trans_;
	frontVec_ -= player_->GetPosition();
	Vector2D vec2(frontVec_.x, frontVec_.z);
	float dis = frontVec_.GetLength();
	mat_.angle_.x = atan2(-frontVec_.y, vec2.GetLength());
	frontVec_.Normalize();
	mat_.angle_.y = atan2(frontVec_.x, frontVec_.z);

	return dis;
}

void Boss::OnCollision(CollisionInfo& info)
{
	(void)info;
	//if (info.GetCollider()->GetAttribute() == CollAttribute::COLLISION_ATTR_ALLIES) {
	//	if (bodyAt_ == true) {
	//		player_->DecHP(10);
	//		Vector3D pos = player_->GetPosition();
	//		Vector3D inter = info.GetInter();
	//		inter.y = 0.0f;
	//		pos -= inter * 3;
	//		player_->SetPosition(pos);
	//	}
	//}
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

bool Boss::GetBodyAttack()
{
	return bodyAt_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void Boss::SetCurrentState(std::unique_ptr<EnemyState>& next)
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

	Vector3D red(1.0f, 0.0f, 0.0f);
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
