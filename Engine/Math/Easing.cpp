#include "Easing.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include <math.h>

using namespace MyMath;

#pragma region lerp

float Easing::GetNormalizedMinVal()
{
	return 0.0f;
}

float Easing::GetNormalizedMaxVal()
{
	return 1.0f;
}

float Easing::lerp(float start, float end, float t)
{
	return start + t * (end - start);
}

MyMath::Vector2D Easing::lerp(const MyMath::Vector2D& start, const MyMath::Vector2D& end, float t)
{
	Vector2D ans;

	ans.x = lerp(start.x, end.x, t);
	ans.y = lerp(start.y, end.y, t);

	return ans;
}

MyMath::Vector3D Easing::lerp(const MyMath::Vector3D& start, const MyMath::Vector3D& end, float t)
{
	Vector3D ans;

	ans.x = lerp(start.x, end.x, t);
	ans.y = lerp(start.y, end.y, t);
	ans.z = lerp(start.z, end.z, t);

	return ans;
}

MyMath::Vector4D Easing::lerp(const MyMath::Vector4D& start, const MyMath::Vector4D& end, float t)
{
	MyMath::Vector4D ans;

	ans.x = lerp(start.x, end.x, t);
	ans.y = lerp(start.y, end.y, t);
	ans.z = lerp(start.z, end.z, t);
	ans.w = lerp(start.w, end.w, t);

	return ans;
}

#pragma endregion

#pragma region EaseIn

float Easing::EaseIn(float start, float end, float t, int32_t powNum)
{
	return lerp(start, end, powf(t, (float)powNum));
}

MyMath::Vector2D Easing::EaseIn(const MyMath::Vector2D& start, const MyMath::Vector2D& end, float t, int32_t powNum)
{
	return lerp(start, end, powf(t, (float)powNum));
}

MyMath::Vector3D Easing::EaseIn(const MyMath::Vector3D& start, const MyMath::Vector3D& end, float t, int32_t powNum)
{
	return lerp(start, end, powf(t, (float)powNum));
}

MyMath::Vector4D Easing::EaseIn(const MyMath::Vector4D& start, const MyMath::Vector4D& end, float t, int32_t powNum)
{
	return lerp(start, end, powf(t, (float)powNum));
}
#pragma endregion

#pragma region EaseOut

float Easing::EaseOut(float start, float end, float t, int32_t powNum)
{
	return lerp(start, end, (1.0f - powf(1.0f - t, (float)powNum)));
}

MyMath::Vector2D Easing::EaseOut(const MyMath::Vector2D& start, const MyMath::Vector2D& end, float t, int32_t powNum)
{
	return lerp(start, end, (1.0f - powf(1.0f - t, (float)powNum)));
}

MyMath::Vector3D Easing::EaseOut(const MyMath::Vector3D& start, const MyMath::Vector3D& end, float t, int32_t powNum)
{
	return lerp(start, end, (1.0f - powf(1.0f - t, (float)powNum)));
}

MyMath::Vector4D Easing::EaseOut(const MyMath::Vector4D& start, const MyMath::Vector4D& end, float t, int32_t powNum)
{
	return lerp(start, end, (1.0f - powf(1.0f - t, (float)powNum)));
}

#pragma endregion

#pragma region EaseInOut

float Easing::EaseInOut(float start, float end, float t, int32_t powNum)
{
	if (t < 0.5f)
	{
		return lerp(start, end, powf(2.0f, powNum - 1.0f) * powf(t, (float)powNum));
	}

	return lerp(start, end, 1.0f - powf(-2.0f * t + 2.0f, (float)powNum) / 2.0f);
}

MyMath::Vector2D Easing::EaseInOut(const MyMath::Vector2D& start, const MyMath::Vector2D& end, float t, int32_t powNum)
{
	if (t < 0.5f)
	{
		return lerp(start, end, powf(2.0f, powNum - 1.0f) * powf(t, (float)powNum));
	}

	return lerp(start, end, 1.0f - powf(-2.0f * t + 2.0f, (float)powNum) / 2);
}

MyMath::Vector3D Easing::EaseInOut(const MyMath::Vector3D& start, const MyMath::Vector3D& end, float t, int32_t powNum)
{
	if (t < 0.5f)
	{
		return lerp(start, end, powf(2.0f, powNum - 1.0f) * powf(t, (float)powNum));
	}

	return lerp(start, end, 1.0f - powf(-2.0f * t + 2.0f, (float)powNum) / 2);
}

MyMath::Vector4D Easing::EaseInOut(const MyMath::Vector4D& start, const MyMath::Vector4D& end, float t, int32_t powNum)
{
	if (t < 0.5f)
	{
		return lerp(start, end, powf(2.0f, powNum - 1.0f) * powf(t, (float)powNum));
	}

	return lerp(start, end, 1.0f - powf(-2.0f * t + 2.0f, (float)powNum) / 2);
}

#pragma endregion

#pragma region EaseInBack

float Easing::EaseInBack(float start, float end, float t, int32_t powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf((c3 * t * t * t - c1 * t * t), (float)powNum));
}

MyMath::Vector2D Easing::EaseInBack(const MyMath::Vector2D& start, const MyMath::Vector2D& end, float t, int32_t powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf((c3 * t * t * t - c1 * t * t), (float)powNum));
}

MyMath::Vector3D Easing::EaseInBack(const MyMath::Vector3D& start, const MyMath::Vector3D& end, float t, int32_t powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf((c3 * t * t * t - c1 * t * t), (float)powNum));
}

MyMath::Vector4D Easing::EaseInBack(const MyMath::Vector4D& start, const MyMath::Vector4D& end, float t, int32_t powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf((c3 * t * t * t - c1 * t * t), (float)powNum));
}

#pragma endregion

#pragma region EaseOutBack

float Easing::EaseOutBack(float start, float end, float t, int32_t powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf(1.0f + c3 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f), (float)powNum));
}

MyMath::Vector2D Easing::EaseOutBack(const MyMath::Vector2D& start, const MyMath::Vector2D& end, float t, int32_t powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf(1.0f + c3 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f), (float)powNum));
}

Vector3D Easing::EaseOutBack(const Vector3D& start, const Vector3D& end, float t, int32_t powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf(1.0f + c3 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f), (float)powNum));
}

MyMath::Vector4D Easing::EaseOutBack(const MyMath::Vector4D& start, const MyMath::Vector4D& end, float t, int32_t powNum, float p1)
{
	float c1 = p1;
	float c3 = c1 + 1;

	return lerp(start, end, powf(1.0f + c3 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f), (float)powNum));
}

#pragma endregion
