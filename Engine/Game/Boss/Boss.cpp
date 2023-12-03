#include "Boss.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "BossIdleState.h"

#include "Player.h"
#include "Quaternion.h"

#include "ImGuiManager.h"

void Boss::StatusInitialize()
{
	hp_.SetMaxHP(maxHP_);

	//	初期ステート
	currentState_ = std::make_unique<BossIdleState>();
	EnemyState::SetBoss(this);

	hp_.Initialize();
}

void Boss::Initialize(IModel* model)
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
	//	死亡判定
	hp_.Update();

	//	生きていたら

	Vector3D dir = player_->GetPosition() - mat_.trans_;
	dir.Normalize();
	frontVec_.Normalize();
	q = DirectionToDirection(frontVec_, dir);
	mat_.matRot_ *= q.GetRotMatrix();
	frontVec_ = dir;

	//Vector3D upVec(0, 1, 0);
	//upVec.Normalize();
	//Vector3D leftVec = upVec.cross(frontVec_);
	//upVec = frontVec_.cross(leftVec);
	//mat_.angle_.x = MyMath::ConvertToRad(-asin(frontVec_.y));
	//mat_.angle_.y = MyMath::ConvertToRad(atan2(frontVec_.x, frontVec_.z));
	//mat_.angle_.z = MyMath::ConvertToRad(atan2(leftVec.y, upVec.y));

	currentState_->Update();

	if (hp_.GetIsAlive() == true) {

		ColliderUpdate();
	}
}

void Boss::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->BeginWindow("PlayerStatus", true);

	imgui->Text("frontVec : (%.2f, %.2f, %.2f)", frontVec_.x, frontVec_.y, frontVec_.z);
	imgui->Text("quaternion : (%.2f, %.2f, %.2f, %.2f)", q.x, q.y, q.z, q.w);

	imgui->EndWindow();
}

void Boss::DrawUI()
{
	hp_.Draw();
}

void Boss::CollisionUpdate()
{
	MatUpdate(false);
	collider_->Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

void Boss::GetDamage(int32_t damage)
{
	hp_.DecHp(damage);
}

bool Boss::GetIsAlive()
{
	return hp_.GetIsAlive();
}

bool Boss::GetIsHide()
{
	return hide_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void Boss::SetCurrentState(std::unique_ptr<EnemyState>& next)
{
	currentState_ = std::move(next);
	currentState_->Initialize();
}

void Boss::SetIsHide(bool isHide)
{
	hide_ = isHide;
}

void Boss::SetPlayer(Player* player)
{
	player_ = player;
}

void Boss::SetHPBarSprite(const Sprite& sprite)
{
	hp_.SetSprite(sprite);

	Vector3D red(1.0f, 0.0f, 0.0f);
	hp_.SetBarColor(red);
}
