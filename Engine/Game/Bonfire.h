#pragma once
#include "Object3D.h"

namespace MNE
{
	
	class ParticleEmitter;

}

class Bonfire :public MNE::Object3D
{
public:
	void Start();
	void Stop();
private:
	MNE::ParticleEmitter* smoke_ = nullptr;
	MNE::ParticleEmitter* fire_ = nullptr;
};

