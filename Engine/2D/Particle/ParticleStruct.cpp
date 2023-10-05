#include "ParticleStruct.h"
#include "Easing.h"

void MoveParticle::Initialize(const Vector3D& pos, const Vector3D& velocity, float scale, const int lifeTime)
{
	Particle::Initialize();

	SetPosition(pos);
	SetScale(3.0f);

	// 向き読み込み
	this->spd = velocity;
	// 大きさ読み込み
	this->maxScale = scale;

	maxTime = lifeTime;
}

void MoveParticle::Update()
{
	SetScale((float)Easing::EaseIn(3.0f, maxScale, (float)timer / (float)maxTime, 2));

	// 移動
	Move(spd);

	// 消える処理
	if (++timer >= maxTime)
	{
		isEnd = true;
	}

	Particle::MatUpdate();
}
