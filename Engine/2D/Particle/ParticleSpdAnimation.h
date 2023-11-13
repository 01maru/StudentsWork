#pragma once
#include "ParticleComponent.h"
#include "Vector3D.h"

class ParticleSpdAnimation :public ParticleComponent
{
public:
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;

private:
	Vector3D spd_;

public:
#pragma region Getter

#pragma endregion

#pragma region Setter

	void SetSpd(const Vector3D& spd);

#pragma endregion
};
