#pragma once
#include "MyMath.h"

/**
* @file PointLight.h
* @brief 点光源ファイル
*/

class PointLight
{
private:
	bool active_ = false;
	Vector3D pos_;
	Vector3D color_ = { 1.0f,1.0f,1.0f };
	Vector3D atten_ = { 1.0f,1.0f,1.0f };

public:
#pragma region Getter

	/**
	* @fn GetIsActive()
	* active_のGetter関数
	* @return active_の値
	*/
	bool GetIsActive() { return active_; }
	/**
	* @fn GetLightPos()
	* pos_のGetter関数
	* @return pos_の値
	*/
	const Vector3D& GetLightPos() { return pos_; }
	/**
	* @fn GetLightColor()
	* color_のGetter関数
	* @return color_の値
	*/
	const Vector3D& GetLightColor() { return color_; }
	/**
	* @fn GetLightAtten()
	* atten_のGetter関数
	* @return atten_の値
	*/
	const Vector3D& GetLightAtten() { return atten_; }

#pragma endregion

#pragma region Setter

	/**
	* @fn SetActive(bool)
	* active_の値を変更するための関数
	* @param active active_の変更後の値
	*/
	void SetActive(bool active) { active_ = active; }
	/**
	* @fn SetLightPos(const Vector3D&)
	* pos_の値を変更するための関数
	* @param pos pos_の変更後の値
	*/
	void SetLightPos(const Vector3D& pos) { pos_ = pos; }
	/**
	* @fn SetLightColor(const Vector3D&)
	* color_の値を変更するための関数
	* @param color color_の変更後の値
	*/
	void SetLightColor(const Vector3D& color) { color_ = color; }
	/**
	* @fn SetLightAtten(const Vector3D&)
	* atten_の値を変更するための関数
	* @param atten atten_の変更後の値
	*/
	void SetLightAtten(const Vector3D& atten) { atten_ = atten; }

#pragma endregion
};

