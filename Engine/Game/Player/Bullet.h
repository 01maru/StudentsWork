#pragma once
#include "Object3D.h"
#include "FrameCounter.h"
#include "ParticleEmitter.h"

class Bullet :public MNE::Object3D
{
public:
	~Bullet();
	void Initialize();
	void Update();

private:
	const int32_t LIFE_TIME = 600;
	Vector3D moveVec_;
	float spd_;
	FrameCounter lifeTime_;
	float radius_ = 0.4f;
	int32_t damage_ = 5;
	int32_t emitterLifeTime_ = 30;
	MNE::ParticleEmitter* emitter_ = nullptr;
public:
	void SetMoveVec(const Vector3D& moveVec);
	void SetSpd(float spd);
	void SetLifeTime(int32_t time);
	bool GetIsActive();
	void OnCollision(CollisionInfo& info) override;
};

