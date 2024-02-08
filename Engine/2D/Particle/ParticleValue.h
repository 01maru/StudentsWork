#pragma once
#include "Vector3D.h"

namespace MNE
{

	class ParticleValue
	{
	public:
		virtual void CalcValue() {};

	protected:
		MyMath::Vector3D value_;

	public:
		MyMath::Vector3D& GetValue();

		void SetValue(const MyMath::Vector3D& v);
	};

	class ParticleRandValue :public ParticleValue
	{
	public:
		void CalcValue() override;

	private:
		MyMath::Vector3D minValue_;
		MyMath::Vector3D maxValue_;

	public:
	#pragma region Getter

	#pragma endregion

	#pragma region Setter

		void SetMinValue(const MyMath::Vector3D& v);
		void SetMaxValue(const MyMath::Vector3D& v);

	#pragma endregion
	};

	class ParticleSameRandValue :public ParticleValue
	{
	public:
		void CalcValue() override;

	private:
		float minValue_;
		float maxValue_;

	public:
	#pragma region Getter

	#pragma endregion

	#pragma region Setter

		void SetMinValue(float v);
		void SetMaxValue(float v);

	#pragma endregion
	};

}
