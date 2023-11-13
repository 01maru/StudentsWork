#include "Bullet.h"
#include "CollisionAttribute.h"
#include "BaseCollider.h"
#include "SphereCollider.h"

void Bullet::Initialize()
{
	Object3D::Initialize();
	lifeTime_.StartCount();
	float scale = radius_ * 2.0f;
	mat_.scale_ = Vector3D(scale, scale, scale);
	SetCollider(new SphereCollider(Vector3D(), radius_));
}

void Bullet::Update()
{
	lifeTime_.Update();

	mat_.trans_ += moveVec_ * spd_;

	MatUpdate();
	collider_->Update();
}

void Bullet::SetMoveVec(const Vector3D& moveVec)
{
	moveVec_ = moveVec;
}

void Bullet::SetSpd(float spd)
{
	spd_ = spd;
}

void Bullet::SetLifeTime(int32_t time)
{
	lifeTime_.SetMaxFrameCount(time);
}

bool Bullet::GetIsActive()
{
	return lifeTime_.GetIsActive();
}

void Bullet::OnCollision(CollisionInfo& info)
{
	(void)info;
	switch (info.GetCollider()->GetAttribute())
	{
	case CollAttribute::COLLISION_ATTR_LANDSHAPE:
		lifeTime_.SetIsActive(false);
		break;
	case CollAttribute::COLLISION_ATTR_ENEMYS:
		//lifeTime_.SetIsActive(false);
		break;
	default:
		break;
	}
}
