#pragma once
#include "EmitterComponent.h"
#include "Vector3D.h"

class EmitterSpdAnimation :public EmitterComponent
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize(Particle* particle) override;

private:
	Vector3D spd_ = { 0.0f,1.0f,0.0f };
};

