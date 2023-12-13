#pragma once
#include "MyMath.h"

/**
* @file ICamera.h
* @brief 継承するカメラファイル
*/

class ICamera
{
public:
	virtual ~ICamera() = default;
	/**
	* @fn Initialize(const Vector3D&, const Vector3D&, float)
	* 初期化用関数
	* @param frontVec 前方方向ベクトル
	* @param center 注視点座標
	* @param dis 注視点と視点座標の距離
	*/
	virtual void Initialize(const Vector3D& frontVec, const Vector3D& center, float dis);
	/**
	* @fn Initialize(const Vector3D&, const Vector3D&, const Vector3D&)
	* 初期化用関数
	* @param eye 視点座標
	* @param target 注視点座標
	* @param up 上方向ベクトル
	*/
	virtual void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up);
	/**
	* @fn Update()
	* 更新処理関数
	*/
	virtual void Update();
	/**
	* @fn ImGuiUpdate()
	* ImGui更新処理関数
	*/
	void ImGuiUpdate();
	/**
	* @fn MatUpdate()
	* ビュー行列更新関数
	*/
	void MatUpdate();

protected:
	Matrix matView_;
	Matrix matProj_;

	float disEyeTarget_ = 0.0f;
	Vector3D eye_;						//	視点座標
	Vector3D target_;					//	注視点座標
	Vector3D up_ = { 0.0f,1.0f,0.0f };	//	上方向ベクトル
	Vector3D frontVec_;
	Vector3D rightVec_;
	Vector3D downVec_;

	//	shake
	bool isShaking_ = false;
	Vector3D move_;
	Vector3D shookEye_;					//	shake後の視点座標
	Vector3D shookTarget_;				//	shake後の注視点座標

	Matrix billboard_;
	Matrix billboardY_;

public:
	/**
	* @fn CalcBillboard()
	* ビルボード計算用関数(視点や注視点の値が変わったら方向ベクトルの更新してから計算する必要あり)
	*/
	void CalcBillboard();
	/**
	* @fn CalcDirectionVec()
	* 方向ベクトル計算関数
	*/
	void CalcDirectionVec();

protected:
	/**
	* @fn ImGuiInfo()
	* ImGuiに表示する情報をまとめる関数
	*/
	virtual void ImGuiInfo() {};

	void ShakeUpdate();
public:

	void SetShake(float min, float max);
	void StopShake();

#pragma region Getter
	
	float GetDisEyeTarget();
	const Vector3D& GetEye();
	const Vector3D& GetTarget();
	const Vector3D& GetUp();
	const Vector3D& GetFrontVec();
	const Vector3D& GetRightVec();
	const Vector3D& GetDownVec();
	const Matrix& GetView();
	const Matrix& GetProjection();
	const Matrix& GetBillboard();
	const Matrix& GetBillboardY();
	Matrix GetViewProj() const { return matView_ * matProj_; }

#pragma endregion

#pragma region Setter

	void SetTarget(const Vector3D& t);
	void SetEye(const Vector3D& e);
	void SetUp(const Vector3D& up);
	//	fovYの単位はラジアン
	void SetProjectionMatrix(int32_t width, int32_t height, float fovY);
	void SetProjMatrix(const Matrix& matProj);

#pragma endregion
};

