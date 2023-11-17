#pragma once

/**
* @file Vector4D.h
* @brief 変数4つあるVectorファイル
*/

class Vector3D;
class Matrix;

class Vector4D
{
public:
	float x;
	float y;
	float z;
	float w;
public:
#pragma region Constructor
	
	Vector4D();
	Vector4D(float x, float y, float z, float w);
	Vector4D(const Vector3D& v, float w);

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
	Vector4D& GetNormalize();

	void SetVec3D(const Vector3D& vec3);
	Vector3D GetVec3D();
	Vector4D operator+() const;
	Vector4D operator-() const;
	Vector4D& operator-=(const Vector4D& v);
	Vector4D& operator/=(float s);
	Vector4D& operator*=(const Matrix& m);

	bool operator!=(const Vector4D& vec);
};

Vector4D operator-(const Vector4D& v1, const Vector4D& v2);