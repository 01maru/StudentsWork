#pragma once
#include "ParticleComponent.h"
#include "Vector3D.h"

class SpriteParticleScaleAnimation :public ParticleComponent
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


class ObjectParticleScaleAnimation :public ParticleComponent
{
public:
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;

private:
	Vector3D start_;
	Vector3D end_;

public:
#pragma region Getter

#pragma endregion

#pragma region Setter

	void SetStartScale(const Vector3D& scale);
	void SetEndScale(const Vector3D& scale);

#pragma endregion
};