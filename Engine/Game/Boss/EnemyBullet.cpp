#include "EnemyBullet.h"
#include "CollisionAttribute.h"
#include "BaseCollider.h"
#include "SphereCollider.h"
#include "Player.h"

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Initialize()
{
	Object3D::Initialize();
	lifeTime_.StartCount();
	float diameter = radius_ * 2.0f;
	mat_.scale_ = Vector3D(diameter, diameter, diameter);
	SetCollider(new SphereCollider(Vector3D(), radius_));
	SetAttribute(CollAttribute::COLLISION_ATTR_ENEMY_AT);
}

void EnemyBullet::Update()
{
	lifeTime_.Update();

	mat_.trans_ += moveVec_ * spd_;

	MatUpdate();
	collider_->Update();
}

void EnemyBullet::SetMoveVec(const Vector3D& moveVec)
{
	moveVec_ = moveVec;
}

void EnemyBullet::SetSpd(float spd)
{
	spd_ = spd;
}

void EnemyBullet::SetLifeTime(int32_t time)
{
	lifeTime_.SetMaxFrameCount(time);
}

bool EnemyBullet::GetIsActive()
{
	return lifeTime_.GetIsActive();
}

void EnemyBullet::OnCollision(CollisionInfo& info)
{
	(void)info;
	Player* player = nullptr;
	std::unique_ptr<FrameCounter> deadTimer;

	switch (info.GetCollider()->GetAttribute())
	{
	case CollAttribute::COLLISION_ATTR_LANDSHAPE:
		//lifeTime_.SetIsActive(false);
		//	ヒット演出
		break;
	case CollAttribute::COLLISION_ATTR_ALLIES:
		player = dynamic_cast<Player*>(info.GetCollider()->GetObject3D());
		player->DecHP(damage_);
		lifeTime_.SetIsActive(false);
		break;
	default:
		break;
	}
}
