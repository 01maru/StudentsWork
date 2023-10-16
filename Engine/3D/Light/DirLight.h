#pragma once
#include "MyMath.h"
#include <memory>

/**
* @file DirLight.h
* @brief 方向ライトファイル
*/

class DirLight
{
private:
	bool active_ = false;
	Vector3D dir_ = { 0,1,0 };
	Vector3D color_ = { 1,1,1 };

	bool shadowing_ = false;
	Vector3D center_ = { 0.0f, 1.0f, 0.0f };

public:
#pragma region Getter

	/**
	* @fn GetIsActive()
	* active_のGetter関数
	* @return active_の値
	*/
	bool GetIsActive() { return active_; }
	/**
	* @fn GetLightDir()
	* dir_のGetter関数
	* @return dir_の値
	*/
	const Vector3D& GetLightDir() { return dir_; }
	/**
	* @fn GetLightColor()
	* color_のGetter関数
	* @return color_の値
	*/
	const Vector3D& GetLightColor() { return color_; }
	/**
	* @fn GetShadowing()
	* shadowing_のGetter関数
	* @return shadowing_の値
	*/
	bool GetShadowing() { return shadowing_; }

#pragma endregion

#pragma region Setter

	/**
	* @fn SetActive(bool)
	* active_の値を変更するための関数
	* @param active active_の変更後の値
	*/
	void SetActive(bool active) { active_ = active; }
	/**
	* @fn SetLightDir(const Vector3D&)
	* dir_をdirの正規化したの値に変更するための関数
	* @param dir active_の変更後の値
	*/
	void SetLightDir(const Vector3D& dir);
	/**
	* @fn SetLightColor(const Vector3D&)
	* color_の値を変更するための関数
	* @param color color_の変更後の値
	*/
	void SetLightColor(const Vector3D& color) { color_ = color; }
	/**
	* @fn SetShadow(bool)
	* 影をつけるかどうか設定する関数
	* @param shadowflag 影をつけるかどうか
	*/
	void SetShadow(bool shadowflag);

#pragma endregion
};

