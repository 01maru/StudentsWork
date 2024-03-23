#include "SuctionEmptyObj.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"

using namespace MyMath;
using namespace MNE;
using namespace MNE::CollAttribute;

SuctionEmptyObj::~SuctionEmptyObj()
{
	if (sphereCol_) {
		sphereCol_->Finalize();
	}
}

void SuctionEmptyObj::Initialize()
{
	mat_.Update();

	sphereCol_ = std::make_unique<SphereCollider>();
	sphereCol_->SetAttribute(COLLISION_ATTR_ENEMY_SUCTION);
	sphereCol_->SetObject3D(this);
	sphereCol_->Update();
}

void SuctionEmptyObj::Update()
{
	sphereCol_->Update();

	CollisionManager::GetInstance()->CheckCollision(sphereCol_.get(), COLLISION_ATTR_ALLIES);
}

void SuctionEmptyObj::OnCollision(MNE::CollisionInfo& info)
{
	if (info.GetCollider()->GetAttribute() & COLLISION_ATTR_ALLIES)
	{
		Vector3D pPos = info.GetCollider()->GetObject3D()->GetPosition();
		Vector3D dir = mat_.trans_ - pPos;
		dir.y = 0.0f;

		dir.Normalize();
		pPos += dir * suctionV_;
		info.GetCollider()->GetObject3D()->SetPosition(pPos);
		info.GetCollider()->GetObject3D()->MatUpdate();
	}
}

float SuctionEmptyObj::GetRadius()
{
	return sphereCol_->GetRadius();
}

void SuctionEmptyObj::SetRadius(float rad)
{
	sphereCol_->SetRadius(rad);
}

void SuctionEmptyObj::SetSuctionValue(float v)
{
	suctionV_ = v;
}
