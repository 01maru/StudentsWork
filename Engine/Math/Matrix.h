#pragma once

/**
* @file Matrix.h
* @brief 4x4行列クラスをまとめたファイル
*/

class Matrix
{
public:
	float m[4][4];

public:
	//	コンストラクタ
	Matrix();

	/**
	* @fn Normalize()
	* 単位行列化用関数
	*/
	void Identity();

	Matrix& operator*=(const Matrix& m2);
};

const Matrix operator*(const Matrix& m1, const Matrix& m2);
//	逆行列
bool InverseMatrix(const Matrix& mat, Matrix& invMat);

//	2D座標系変換用行列
Matrix Create2DTransformMatrix();
