#include "ParticleStruct.h"
#include "Easing.h"

void MoveParticle::Initialize(const Vector3D& pos, const Vector3D& velocity, float scale, const int lifeTime)
{
	SetPosition(pos);
	SetScale(3.0f);

	// �����ǂݍ���
	this->spd = velocity;
	// �傫���ǂݍ���
	this->maxScale = scale;

	maxTime = lifeTime;
}

void MoveParticle::Update()
{
	SetScale((float)Easing::EaseIn(3.0f, maxScale, (float)timer / (float)maxTime, 2));

	// �ړ�
	Move(spd);

	// �����鏈��
	if (++timer >= maxTime)
	{
		isEnd = true;
	}
}
