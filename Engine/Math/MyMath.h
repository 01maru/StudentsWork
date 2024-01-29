#pragma once
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"
#include <vector>

/**
* @file MyMath.h
* @brief 数学に関する処理をまとめたファイル
*/

namespace MyMath {
	/**
	* @fn mMax(T, T)
	* 引数を比べ大きい方の値を返す関数
	* @return 引数の大きい方の値
	*/
	template<typename T>
	T mMax(T a, T b);
	/**
	* @fn mMin(T, T)
	* 引数を比べ小さい方の値を返す関数
	* @return 引数の小さい方の値
	*/
	template<typename T>
	T mMin(T a, T b);
	/**
	* @fn mMin(int32_t, int32_t, int32_t)
	* valueの値をloopする用関数
	* @param maxValue ループの最大値
	* @param minValue ループの最小値
	* @param value ループさせる値
	* @return ループ処理後のvalueの値
	*/
	int32_t mLoop(int32_t maxValue, int32_t minValue, int32_t value);
	/**
	* @fn mClamp(float, float, float)
	* valueの値をloopする用関数
	* @param minValue Clampの最小値
	* @param maxValue Clampの最大値
	* @param value Clampさせる値
	* @return Clamp処理後のvalueの値
	*/
	float mClamp(float minValue, float maxValue, float value);
	Vector2D mClamp(const Vector2D& minValue, const Vector2D& maxValue, const Vector2D& value);

	Vector3D BezierCurve(const Vector3D& start, const Vector3D& controlPoint, const Vector3D& end, float t);

#pragma region 円周率
	const float PI = 3.14159265358979f;
	const float PIx2 = 6.2831853071f;
#pragma endregion
	//	ガウシアンブラー用のWeight計算
	void CalcGaussianWeightsTable(std::vector<float>& weights, float sigma);

	//	視点座標、注視点座標、上方向ベクトル
	Matrix LookAtLH(const Vector3D& eye, const Vector3D& target, const Vector3D& up);

	Matrix PerspectiveFovLH(int32_t winwidth, int32_t winheight, float fovY, float nearZ, float farZ);
	Matrix OrthoLH(int32_t winwidth, int32_t winheight, float nearZ, float farZ);

	/**
	* @fn ConvertToRad(float)
	* ラジアン変換関数
	* @param angle 度数法の角度
	* @return 弧度法(ラジアン)の角度
	*/
	float ConvertToRad(float angle);

	/**
	* @fn GetRand(float, float)
	* 乱数関数
	* @param min 乱数最大値
	* @param max 乱数最小値
	* @return 乱数値
	*/
	float GetRand(float min, float max);

	Vector2D GetRand(const Vector2D& minV, const Vector2D& maxV);
	Vector3D GetRand(const Vector3D& minV, const Vector3D& maxV);

	bool CollisionCircleLay(const Vector3D& startL, const Vector3D& endL, const Vector3D& pos, float rad);

	bool CollisionSquareToPoint(const Vector2D& leftTop, const Vector2D& rightBottom, const Vector2D& point);
	/**
	* @fn CreatePolygonNormal(const Vector3D&, const Vector3D&, const Vector3D&)
	* 頂点ABCで作られたポリゴンから法線を計算する用関数
	* @return 頂点ABCポリゴンの法線の値
	*/
	Vector3D CreatePolygonNormal(const Vector3D& a, const Vector3D& b, const Vector3D& c);

	class MatView {
	public:
		Matrix mat_;
		Vector3D eye_;		//	視点座標
		Vector3D target_;	//	注視点座標
		Vector3D up_;		//	上方向ベクトル
	public:
		MatView();
		void Init(const Vector3D& _eye, const Vector3D& _target, const Vector3D& _up);
		void MatUpdate();
	};

	class ObjMatrix {
	public:
		Matrix matWorld_;

		Matrix matScale_;
		Vector3D scale_;		//	スケール

		Matrix matRot_;
		Vector3D angle_;	//	回転(単位はラジアン)

		Matrix matTrans_;
		Vector3D trans_;		//	位置

	public:
		void SetMatScaling();
		void SetMatRotation();
		void SetMatTransform();
		void Initialize();
		void Update(bool calcRot = true);
	};

	class SpriteMatrix
	{
	public:
		Matrix matWorld_;

		Matrix matRot_;
		float rotation_ = 0.0f;

		Matrix matTrans_;
		Vector2D trans_;
	public:
		void SetMatRotation();
		void SetMatTransform();
		void Update();

		const Matrix& GetMatWorld() { return matWorld_; }
		float GetAngle() { return rotation_; }
		Vector2D& GetTrans() { return trans_; }

		void SetAngle(float angle) { rotation_ = angle; }
		void SetTrans(const Vector2D& trans) { trans_ = trans; }
	};
}

