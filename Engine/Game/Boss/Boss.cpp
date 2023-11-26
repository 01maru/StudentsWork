#include "Boss.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "Easing.h"
#include "BossIdleState.h"

#include "TextureManager.h"
#include "Player.h"
#include "Quaternion.h"

void Boss::StatusInitialize()
{
	//	初期ステート
	currentState_ = std::make_unique<BossIdleState>();
	EnemyState::SetBoss(this);

	hp_.Initialize();
}

void Boss::Initialize(IModel* model)
{
	hp_.SetMaxHP(maxHP_);
	Sprite hpBar;
	hpBar.Initialize();
	hpBar.SetPosition({ 240, 70 });
	hpBar.SetAnchorPoint({ 0.0f, 0.5f });
	hpBar.SetSize({ 800, 40 });
	hp_.SetSprite(hpBar);

	Object3D::Initialize();
	SetModel(model);
	mat_.trans_.y = 3.0f;
	mat_.trans_.z = 10.0f;

	StatusInitialize();

	float radius = 3.0f;
	SetCollider(new SphereCollider(Vector3D(0, 0, 0), radius));
	collider_->SetAttribute(CollAttribute::COLLISION_ATTR_ENEMYS);
}

void Boss::Update()
{
	//	死亡判定
	hp_.Update();

	//	生きていたら

	//Vector3D dir = player_->GetPosition() - mat_.trans_;
	//dir.Normalize();
	//frontVec_.Normalize();
	//Quaternion q = DirectionToDirection(frontVec_, dir);
	//frontVec_ = q.GetVector3().GetNormalize();

	//Vector3D upVec(-1, 1, 0);
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

void Boss::DrawUI()
{
	hp_.Draw();
}

void Boss::CollisionUpdate()
{
	MatUpdate();
	collider_->Update();
}

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
