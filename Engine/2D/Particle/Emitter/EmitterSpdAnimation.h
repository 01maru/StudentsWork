#pragma once
#include "EmitterComponent.h"

class EmitterSpdAnimation :public EmitterComponent
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize(Particle* particle) override;
	
private:
	float spd_ = 0.05f;
public:
	void SetSpd(float spd) { spd_ = spd; }
};

