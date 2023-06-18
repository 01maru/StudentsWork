#pragma once
#include "Vector3D.h"
#include "Particle.h"

class MoveParticle :public Particle
{
public:
	void Initialize(const Vector3D& pos,
		const Vector3D& velocity,
		float scale_,
		const int lifeTime);
	void Update();

	bool GetIsEnd() { return isEnd; }
private:
	Vector3D spd;	//	���x

	float maxScale = 0;	//	�ő�X�P�[��
	int timer = 0;		//	���̎���
	int maxTime = 0;	//	��������
	bool isEnd = false;
};

