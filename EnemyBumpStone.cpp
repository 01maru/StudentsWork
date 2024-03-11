#include "EnemyBumpStone.h"

EnemyBumpStone::~EnemyBumpStone()
{
}

void EnemyBumpStone::Initialize()
{
	IBullet::Initialize();
}

void EnemyBumpStone::Update()
{
	lifeTime_.Update();

	mat_.scale_.y = height_ * lifeTime_.GetCountPerMaxCount();

	MatUpdate();
	ColliderUpdate();
}

void EnemyBumpStone::OnCollision(MNE::CollisionInfo& /*info*/)
{
}

void EnemyBumpStone::SetScale(const MyMath::Vector3D& scale)
{
	mat_.scale_ = scale;
	height_ = scale.y;
}
