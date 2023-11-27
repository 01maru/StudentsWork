#pragma once
#include "EmitterComponent.h"

class EmitterLifeTime :public EmitterComponent
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize(Particle* particle) override;

private:
	int32_t lifetime = 60;
public:
	void SetLifeTime(int32_t time) { lifetime = time; }
};

