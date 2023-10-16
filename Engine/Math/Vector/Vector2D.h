#pragma once
#include <cstdint>

/**
* @file Vector2D.h
* @brief 変数2つあるVectorファイル
*/

class Vector2D
{
public:
	float x;
	float y;
public:
#pragma region Constructor

	Vector2D() :x(0), y(0) {}
	Vector2D(float x, float y) :x(x), y(y) {}
	Vector2D(int32_t x, int32_t y) :x((float)x), y((float)y) {}

#pragma endregion

	/**
	* @fn GetLength()
	* ベクトルの長さを返す関数
	* @return ベクトルの長さ
	*/
	float GetLength() const;

	/**
	* @fn Normalize()
	* 正規化用関数
	*/
	void Normalize();
	/**
	* @fn GetNormalize()
	* 正規化したベクトルを返す関数
	* @return 正規化したベクトル
	*/
	Vector2D& GetNormalize();

	Vector2D operator+() const;
	Vector2D& operator+=(const Vector2D& v);
	Vector2D& operator-=(const Vector2D& v);
	Vector2D& operator/=(float s);
	Vector2D& operator*=(float s);

	bool operator!=(const Vector2D& vec);
	bool operator==(const Vector2D& vec);
};

Vector2D operator-(const Vector2D& v1, const Vector2D& v2);
Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
Vector2D operator*(const Vector2D& v, float s);
Vector2D operator*(float s, const Vector2D& v);
