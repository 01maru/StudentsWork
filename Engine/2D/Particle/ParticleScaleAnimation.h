#pragma once
#include "ParticleComponent.h"
#include "Vector3D.h"

namespace MNE
{

	class SpriteParticleScaleAnimation :public MNE::ParticleComponent
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


	class ObjectParticleScaleAnimation :public MNE::ParticleComponent
	{
	public:
		/**
		* @fn Update()
		* 更新処理関数
		*/
		void Update() override;

	private:
		MyMath::Vector3D start_;
		MyMath::Vector3D end_;

	public:
	#pragma region Getter

	#pragma endregion

	#pragma region Setter

		void SetStartScale(const MyMath::Vector3D& scale);
		void SetEndScale(const MyMath::Vector3D& scale);

	#pragma endregion
	};

}