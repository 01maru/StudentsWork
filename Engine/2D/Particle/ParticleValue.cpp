#include "ParticleValue.h"
#include "MyMath.h"

using namespace MyMath;

Vector3D& MNE::ParticleValue::GetValue()
{
	CalcValue();

	return value_;
}

void MNE::ParticleValue::SetValue(const Vector3D& v)
{
	value_ = v;
}

void MNE::ParticleRandValue::CalcValue()
{
	value_ = GetRand(minValue_, maxValue_);
}

void MNE::ParticleRandValue::SetMinValue(const Vector3D& v)
{
	minValue_ = v;
}

void MNE::ParticleRandValue::SetMaxValue(const Vector3D& v)
{
	maxValue_ = v;
}

void MNE::ParticleSameRandValue::CalcValue()
{
	float v = GetRand(minValue_, maxValue_);
	value_ = Vector3D(v, v, v);
}

void MNE::ParticleSameRandValue::SetMinValue(float v)
{
	minValue_ = v;
}

void MNE::ParticleSameRandValue::SetMaxValue(float v)
{
	maxValue_ = v;
}
