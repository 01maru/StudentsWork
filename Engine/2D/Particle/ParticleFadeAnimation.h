#pragma once
#include "ParticleComponent.h"

class ParticleFadeAnimation :public ParticleComponent
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

	void SetStartAlpha(float alpha);
	void SetEndAlpha(float alpha);

#pragma endregion
};

