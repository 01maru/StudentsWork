#pragma once
#include "ParticleComponent.h"

class ParticleScaleAnimation :public ParticleComponent
{
public:
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;

private:
	float start_ = 1.0f;
	float end_ = 5.0f;

public:
#pragma region Getter

#pragma endregion

#pragma region Setter

	void SetStartScale(float scale);
	void SetEndScale(float scale);

#pragma endregion
};
