#pragma once
#include "Object3D.h"

class ParticleEmitter;

class Bonfire :public Object3D
{
public:
	void Start();
	void Stop();
private:
	ParticleEmitter* smoke_ = nullptr;
	ParticleEmitter* fire_ = nullptr;
};

