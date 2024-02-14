#include "IBullet.h"
#include "SphereCollider.h"

using namespace MyMath;
using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void IBullet::Initialize()
{
	Object3D::Initialize();
	
	//	生成時間
	lifeTime_.StartCount();

	//	Collider
	float diameter = radius_ * 2.0f;
	mat_.scale_ = Vector3D(diameter, diameter, diameter);
	SetCollider(new SphereCollider(Vector3D(), radius_));
}

void IBullet::Update()
{
	lifeTime_.Update();

	mat_.trans_ += moveVec_ * spd_;
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool IBullet::GetIsActive()
{
	return lifeTime_.GetIsActive();
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void IBullet::SetMoveVec(const MyMath::Vector3D& moveVec)
{
	moveVec_ = moveVec;
}

void IBullet::SetSpd(float spd)
{
	spd_ = spd;
}

void IBullet::SetLifeTime(int32_t time)
{
	lifeTime_.SetMaxFrameCount(time);
}
