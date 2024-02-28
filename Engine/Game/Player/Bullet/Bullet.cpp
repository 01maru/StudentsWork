#include "Bullet.h"
#include "CollisionAttribute.h"
#include "BaseCollider.h"
#include "SphereCollider.h"
#include "Boss.h"

#include "ParticleManager.h"
#include "BulletAfterImgEmitter.h"
#include "BulletHitEmitter.h"
#include "EmitterConeType.h"

using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Destructor
//-----------------------------------------------------------------------------

Bullet::~Bullet()
{
	if (emitter_ != nullptr) {
		emitter_->SetIsDead(true);
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void Bullet::Initialize()
{
	IBullet::Initialize();

	BulletAfterImgEmitter afterImgEmitter;
	emitter_ = ParticleManager::GetInstance()->AddEmitter(afterImgEmitter.GetEmitter());
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void Bullet::Update()
{
	IBullet::Update();

	emitter_->SetPosition(mat_.trans_);

	MatUpdate();
	ColliderUpdate();
}

void Bullet::OnCollision(CollisionInfo& info)
{
	(void)info;
	Boss* boss = nullptr;
	BulletHitEmitter emitter;
	ParticleEmitter* hitEmitter;
	EmitterConeType* cone;
	std::unique_ptr<FrameCounter> deadTimer;

	switch (info.GetCollider()->GetAttribute())
	{
	case CollAttribute::COLLISION_ATTR_LANDSHAPE:
		//lifeTime_.SetIsActive(false);
		//	ヒット演出
		break;
	case CollAttribute::COLLISION_ATTR_ENEMYS:
		boss = dynamic_cast<Boss*>(info.GetCollider()->GetObject3D());
		boss->DecHP(damage_);
		lifeTime_.SetIsActive(false);
		hitEmitter = ParticleManager::GetInstance()->AddEmitter(emitter.GetEmitter());
		cone = dynamic_cast<EmitterConeType*>(hitEmitter->GetEmitterType());
		cone->SetDir(-moveVec_);
		hitEmitter->SetPosition(mat_.trans_);
		deadTimer = std::make_unique<FrameCounter>();
		deadTimer->Initialize(emitterLifeTime_, true);
		hitEmitter->SetDeadTimer(deadTimer);
		break;
	default:
		break;
	}
}
