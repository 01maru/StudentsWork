#pragma once
#include "EmitterComponent.h"

class EmitterSpdComp :public EmitterComponent
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize(Particle* particle) override;
};

