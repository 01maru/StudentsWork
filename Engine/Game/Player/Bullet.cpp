#include "Bullet.h"
#include "CollisionAttribute.h"
#include "BaseCollider.h"
#include "SphereCollider.h"
#include "Boss.h"

#include "ParticleManager.h"
#include "BulletAfterImgEmitter.h"
#include "BulletHitEmitter.h"
#include "EmitterConeType.h"

Bullet::~Bullet()
{
	if (emitter_ != nullptr) {
		emitter_->SetIsDead(true);
	}
}

void Bullet::Initialize()
{
	Object3D::Initialize();
	lifeTime_.StartCount();
	float scale = radius_ * 2.0f;
	mat_.scale_ = Vector3D(scale, scale, scale);
	SetCollider(new SphereCollider(Vector3D(), radius_));

	BulletAfterImgEmitter afterImgEmitter;
	emitter_ = ParticleManager::GetInstance()->AddEmitter(afterImgEmitter.GetEmitter());
}

void Bullet::Update()
{
	lifeTime_.Update();

	mat_.trans_ += moveVec_ * spd_;

	emitter_->SetPosition(mat_.trans_);

	MatUpdate();
	collider_->Update();
}

void Bullet::SetMoveVec(const Vector3D& moveVec)
{
	moveVec_ = moveVec;
	if (emitter_ != nullptr) {
		emitter_->GetEmitterType()->SetDir(moveVec);
	}
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
		boss->GetDamage(damage_);
		lifeTime_.SetIsActive(false);
		hitEmitter = ParticleManager::GetInstance()->AddEmitter(emitter.GetEmitter());
		cone = dynamic_cast<EmitterConeType*>(hitEmitter->GetEmitterType());
		cone->SetConeDir(-moveVec_);
		hitEmitter->SetPosition(mat_.trans_);
		deadTimer = std::make_unique<FrameCounter>();
		deadTimer->Initialize(30, true);
		hitEmitter->SetDeadTimer(deadTimer);
		break;
	default:
		break;
	}
}
