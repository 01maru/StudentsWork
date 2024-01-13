#pragma once
#include "Vector3D.h"

namespace MNE
{

	class ParticleValue
	{
	public:
		virtual void CalcValue() {};

	protected:
		Vector3D value_;

	public:
		Vector3D& GetValue();

		void SetValue(const Vector3D& v);
	};

	class ParticleRandValue :public ParticleValue
	{
	public:
		void CalcValue() override;

	private:
		Vector3D minValue_;
		Vector3D maxValue_;

	public:
	#pragma region Getter

	#pragma endregion

	#pragma region Setter

		void SetMinValue(const Vector3D& v);
		void SetMaxValue(const Vector3D& v);

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
