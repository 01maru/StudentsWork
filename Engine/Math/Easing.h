#pragma once
#include <cstdint>

/**
* @file Easing.h
* @brief イージング関数をまとめたファイル
*/

class Vector2D;
class Vector3D;
class Vector4D;

namespace Easing {

	enum PawValue
	{
		Single = 1,
		Double,
		Triple,
		Quad,
		Quint,
	};

#pragma region lerp

	/**
	* @fn lerp(float, float, float)
	* lerpした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @return lerpした値(0.0f～1.0f)
	*/
	float lerp(float start, float end, float t);
	/**
	* @fn lerp(const Vector2D&, const Vector2D&, float)
	* lerpした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @return lerpした値(0.0f～1.0f)
	*/
	Vector2D lerp(const Vector2D& start, const Vector2D& end, float t);
	/**
	* @fn lerp(const Vector3D&, const Vector3D&, float)
	* lerpした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @return lerpした値(0.0f～1.0f)
	*/
	Vector3D lerp(const Vector3D& start, const Vector3D& end, float t);
	/**
	* @fn lerp(const Vector4D&, const Vector4D&, float)
	* lerpした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @return lerpした値(0.0f～1.0f)
	*/
	Vector4D lerp(const Vector4D& start, const Vector4D& end, float t);

#pragma endregion

#pragma region EaseIn

	/**
	* @fn EaseIn(float, float, float, int32_t)
	* EaseInした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseInした値
	*/
	float EaseIn(float start, float end, float t, int32_t powNum);
	/**
	* @fn EaseIn(const Vector2D&, const Vector2D&, float, int32_t)
	* EaseInした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseInした値
	*/
	Vector2D EaseIn(const Vector2D& start, const Vector2D& end, float t, int32_t powNum);
	/**
	* @fn EaseIn(const Vector3D&, const Vector3D&, float, int32_t)
	* EaseInした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseInした値
	*/
	Vector3D EaseIn(const Vector3D& start, const Vector3D& end, float t, int32_t powNum);
	/**
	* @fn EaseIn(const Vector4D&, const Vector4D&, float, int32_t)
	* EaseInした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseInした値
	*/
	Vector4D EaseIn(const Vector4D& start, const Vector4D& end, float t, int32_t powNum);

#pragma endregion

#pragma region EaseOut

	/**
	* @fn EaseOut(float, float, float, int32_t)
	* EaseOutした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseOutした値
	*/
	float EaseOut(float start, float end, float t, int32_t powNum);
	/**
	* @fn EaseOut(const Vector2D&, const Vector2D&, float, int32_t)
	* EaseOutした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseOutした値
	*/
	Vector2D EaseOut(const Vector2D& start, const Vector2D& end, float t, int32_t powNum);
	/**
	* @fn EaseOut(const Vector3D&, const Vector3D&, float, int32_t)
	* EaseOutした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseOutした値
	*/
	Vector3D EaseOut(const Vector3D& start, const Vector3D& end, float t, int32_t powNum);
	/**
	* @fn EaseOut(const Vector4D&, const Vector4D&, float, int32_t)
	* EaseOutした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseOutした値
	*/
	Vector4D EaseOut(const Vector4D& start, const Vector4D& end, float t, int32_t powNum);

#pragma endregion

#pragma region EaseInOut

	/**
	* @fn EaseInOut(float, float, float, int32_t)
	* EaseInOutした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseInOutした値
	*/
	float EaseInOut(float start, float end, float t, int32_t powNum);
	/**
	* @fn EaseInOut(const Vector2D&, const Vector2D&, float, int32_t)
	* EaseInOutした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseInOutした値
	*/
	Vector2D EaseInOut(const Vector2D& start, const Vector2D& end, float t, int32_t powNum);
	/**
	* @fn EaseInOut(const Vector3D&, const Vector3D&, float, int32_t)
	* EaseInOutした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseInOutした値
	*/
	Vector3D EaseInOut(const Vector3D& start, const Vector3D& end, float t, int32_t powNum);
	/**
	* @fn EaseInOut(const Vector4D&, const Vector4D&, float, int32_t)
	* EaseInOutした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseInOutした値
	*/
	Vector4D EaseInOut(const Vector4D& start, const Vector4D& end, float t, int32_t powNum);

#pragma endregion

#pragma region EaseInBack

	/**
	* @fn EaseInBack(float, float, float, int32_t, float)
	* EaseInBackした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseInBackした値
	*/
	float EaseInBack(float start, float end, float t, int32_t powNum = 1, float p1 = 1.70158f);
	/**
	* @fn EaseInBack(const Vector2D&, const Vector2D&, float, int32_t, float)
	* EaseInBackした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseInBackした値
	*/
	Vector2D EaseInBack(const Vector2D& start, const Vector2D& end, float t, int32_t powNum = 1, float p1 = 1.70158f);
	/**
	* @fn EaseInBack(const Vector3D&, const Vector3D&, float, int32_t, float)
	* EaseInBackした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseInBackした値
	*/
	Vector3D EaseInBack(const Vector3D& start, const Vector3D& end, float t, int32_t powNum = 1, float p1 = 1.70158f);
	/**
	* @fn EaseInBack(const Vector4D&, const Vector4D&, float, int32_t, float)
	* EaseInBackした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseInBackした値
	*/
	Vector4D EaseInBack(const Vector4D& start, const Vector4D& end, float t, int32_t powNum = 1, float p1 = 1.70158f);

#pragma endregion

#pragma region EaseOutBack

	/**
	* @fn EaseOutBack(float, float, float, int32_t, float)
	* EaseOutBackした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseOutBackした値
	*/
	float EaseOutBack(float start, float end, float t, int32_t powNum = 1, float p1 = 1.70158f);
	/**
	* @fn EaseOutBack(const Vector2D&, const Vector2D&, float, int32_t, float)
	* EaseOutBackした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseOutBackした値
	*/
	Vector2D EaseOutBack(const Vector2D& start, const Vector2D& end, float t, int32_t powNum = 1, float p1 = 1.70158f);
	/**
	* @fn EaseOutBack(const Vector3D&, const Vector3D&, float, int32_t, float)
	* EaseOutBackした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseOutBackした値
	*/
	Vector3D EaseOutBack(const Vector3D& start, const Vector3D& end, float t, int32_t powNum = 1, float p1 = 1.70158f);
	/**
	* @fn EaseOutBack(const Vector4D&, const Vector4D&, float, int32_t, float)
	* EaseOutBackした値を返す関数
	* @param start 開始位置
	* @param end 終了位置
	* @param t 割合値(0.0f～1.0f)
	* @param powNum イージングの強さ変更用(正常に動くのは1～5)
	* @return EaseOutBackした値
	*/
	Vector4D EaseOutBack(const Vector4D& start, const Vector4D& end, float t, int32_t powNum = 1, float p1 = 1.70158f);

#pragma endregion
}

