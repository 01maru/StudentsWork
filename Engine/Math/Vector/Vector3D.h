#pragma once
#include <cstdint>

/**
* @file Vector3D.h
* @brief 変数3つあるVectorファイル
*/

class Matrix;
class Vector2D;
class Vector4D;

class Vector3D
{
public:
	float x;
	float y;
	float z;

public:
#pragma region Constructor

	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(int32_t x, int32_t y, int32_t z);
	Vector3D(const Vector2D& vec, float z);
	Vector3D(const Vector2D& vec, int32_t z);
	Vector3D(const Vector4D& vec);

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
	Vector3D& GetNormalize();

	/**
	* @fn dot(const Vector3D&)
	* このベクトルと引数ベクトルの内積を返す関数
	* @param v 内積を求めたいベクトル
	* @return このベクトルと引数ベクトルの内積
	*/
	float dot(const Vector3D& v) const;

	/**
	* @fn cross(const Vector3D&)
	* このベクトルと引数ベクトルの外積を返す関数
	* @param v 外積を求めたいベクトル
	* @return このベクトルと引数ベクトルの外積
	*/
	Vector3D cross(const Vector3D& v) const;

	Vector3D operator+() const;
	Vector3D operator-() const;
	Vector3D& operator+=(const Vector3D& v);
	Vector3D& operator-=(const Vector3D& v);
	Vector3D& operator/=(float s);
	Vector3D& operator*=(float s);

	bool operator==(const Vector3D& vec) const;
	bool operator!=(const Vector3D& vec) const;
};

Vector3D operator+(const Vector3D& v1, const Vector3D& v2);
Vector3D operator-(const Vector3D& v1, const Vector3D& v2);
Vector3D operator*(const Vector3D& v, float s);
Vector3D operator*(float s, const Vector3D& v);

Vector3D Vec3Transform(const Vector3D& v, const Matrix& m);
Vector3D Vec3TransformNormal(const Vector3D& v, const Matrix& m);
