#pragma once
#include "MyMath.h"

/**
* @file CircleShadow.h
* @brief 丸影ファイル
*/

namespace MNE
{

	class CircleShadow
	{
	private:
		bool active_ = false;
		MyMath::Vector3D dir_ = { 1,0,0 };
		MyMath::Vector3D casterPos_ = { 0,0,0 };
		float disCasterLight_ = 100.0f;
		MyMath::Vector3D atten_ = { 0.5f,0.6f,0.0f };
		MyMath::Vector2D factorAngleCos_ = { 0.2f,0.5f };

	public:

	#pragma region Getter

		/**
		* @fn GetIsActive()
		* active_のGetter関数
		* @return active_の値
		*/
		bool GetIsActive() { return active_; }
		/**
		* @fn GetDir()
		* dir_のGetter関数
		* @return dir_の値
		*/
		const MyMath::Vector3D& GetDir() { return dir_; }
		/**
		* @fn GetCasterPos()
		* casterPos_のGetter関数
		* @return casterPos_の値
		*/
		const MyMath::Vector3D& GetCasterPos() { return casterPos_; }
		/**
		* @fn GetDistanceCasterLight()
		* disCasterLight_のGetter関数
		* @return disCasterLight_の値
		*/
		float GetDistanceCasterLight() { return disCasterLight_; }
		/**
		* @fn GetAtten()
		* atten_のGetter関数
		* @return atten_の値
		*/
		const MyMath::Vector3D& GetAtten() { return atten_; }
		/**
		* @fn GetFactorAngleCos()
		* factorAngleCos_のGetter関数
		* @return factorAngleCos_の値
		*/
		const MyMath::Vector2D& GetFactorAngleCos() { return factorAngleCos_; }

	#pragma endregion

	#pragma region Setter

		/**
		* @fn SetActive(bool)
		* active_の値を変更するための関数
		* @param active active_の変更後の値
		*/
		void SetActive(bool active) { active_ = active; }
		/**
		* @fn SetDir(const Vector3D&)
		* dir_をdirの正規化したの値に変更するための関数
		* @param dir dir_の変更後の値
		*/
		void SetDir(const MyMath::Vector3D& dir);
		/**
		* @fn SetCasterPos(const Vector3D&)
		* casterPos_の値を変更するための関数
		* @param casterPos casterPos_の変更後の値
		*/
		void SetCasterPos(const MyMath::Vector3D& casterPos) { casterPos_ = casterPos; }
		/**
		* @fn SetDistanceCasterLight(float)
		* disCasterLight_の値を変更するための関数
		* @param distanceCasterLight disCasterLight_の変更後の値
		*/
		void SetDistanceCasterLight(float distanceCasterLight) { disCasterLight_ = distanceCasterLight; }
		/**
		* @fn SetAtten(const Vector3D&)
		* atten_の値を変更するための関数
		* @param atten atten_の変更後の値
		*/
		void SetAtten(const MyMath::Vector3D& atten) { atten_ = atten; }
		/**
		* @fn SetFactorAngle(const Vector2D&)
		* factorAngle_の値を変更するための関数
		* @param factorAngle factorAngle_の変更後の値
		*/
		void SetFactorAngle(const MyMath::Vector2D& factorAngle);

	#pragma endregion
	};

}

