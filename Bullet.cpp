#include "Bullet.h"

void Bullet::Initialize()
{
	Object3D::Initialize();
	lifeTime_.StartCount();
}

void Bullet::Update()
{
	lifeTime_.Update();

	mat_.trans_ += moveVec_ * spd_;
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
