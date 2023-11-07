#pragma once
#include "EmitterComponent.h"
#include "ParticleValue.h"

class EmitterPosComp :public EmitterComponent
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize(Particle* particle) override;

private:
	std::unique_ptr<ParticleValue> add_;

public:
	void SetAddValue(std::unique_ptr<ParticleValue>& v);
};

