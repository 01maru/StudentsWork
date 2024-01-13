#pragma once
#include "MyMath.h"

/**
* @file SpotLight.h
* @brief スポットライトファイル
*/

namespace MNE
{

	class SpotLight
	{
	private:
		bool active_ = false;
		Vector3D dir_ = { 1,0,0 };
		Vector3D pos_ = { 0,0,0 };
		Vector3D color_ = { 1,1,1 };
		Vector3D atten_ = { 1,1,1 };
		Vector2D factorAngleCos_ = { 0.5f,0.2f };

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
		/**
		* @fn GetLightFactorAngleCos()
		* factorAngleCos_のGetter関数
		* @return factorAngleCos_の値
		*/
		const Vector2D& GetLightFactorAngleCos() { return factorAngleCos_; }

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
		* @param dir dir_の変更後の値
		*/
		void SetLightDir(const Vector3D& dir);
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
		/**
		* @fn SetLightFactorAngle(const Vector2D&)
		* factorAngle_の値を変更するための関数
		* @param factorAngle factorAngle_の変更後の値
		*/
		void SetLightFactorAngle(const Vector2D& factorAngle);

	#pragma endregion
	};

}

