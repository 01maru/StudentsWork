#pragma once
#include "EmitterComponent.h"
#include "Vector3D.h"

class EmitterColor :public EmitterComponent
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize(Particle* particle) override;

private:
	Vector3D color_;

public:
	void SetColor(const Vector3D& color);
};

