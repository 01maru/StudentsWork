#pragma once
#include "ICamera.h"

/**
* @file NormalCamera.h
* @brief 通常時のカメラファイル
*/

class NormalCamera :public ICamera
{
private:
	/**
	* @fn ImGuiInfo()
	* ImGuiに表示する情報をまとめる関数
	*/
	void ImGuiInfo() override;

public:
	NormalCamera() {};
	~NormalCamera() override {};

	/**
	* @fn Initialize(const Vector3D&, const Vector3D&, float)
	* 初期化用関数
	* @param frontVec 前方方向ベクトル
	* @param center 注視点座標
	* @param dis 注視点と視点座標の距離
	*/
	void Initialize(const Vector3D& frontVec, const Vector3D& center, float dis) override;
	/**
	* @fn Initialize(const Vector3D&, const Vector3D&, const Vector3D&)
	* 初期化用関数
	* @param eye 視点座標
	* @param target 注視点座標
	* @param up 上方向ベクトル
	*/
	void Initialize(const Vector3D& eye, const Vector3D& target, const Vector3D& up) override;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;
};
