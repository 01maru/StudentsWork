#include "ParticleValue.h"
#include "MyMath.h"

using namespace MyMath;

Vector3D& ParticleValue::GetValue()
{
	CalcValue();

	return value_;
}

void ParticleValue::SetValue(const Vector3D& v)
{
	value_ = v;
}

void ParticleRandValue::CalcValue()
{
	value_ = GetRand(minValue_, maxValue_);
}

void ParticleRandValue::SetMinValue(const Vector3D& v)
{
	minValue_ = v;
}

void ParticleRandValue::SetMaxValue(const Vector3D& v)
{
	maxValue_ = v;
}
