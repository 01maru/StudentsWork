#pragma once
#include "EmitterComponent.h"
#include "ParticleValue.h"

class EmitterFadeAnimation :public EmitterComponent
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize(Particle* particle) override;

private:
	float minValue_ = 0.0f;
	float maxValue_ = 1.0f;

public:
	void SetMinValue(float v);
	void SetMaxValue(float v);
};

