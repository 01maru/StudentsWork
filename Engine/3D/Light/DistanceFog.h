#pragma once
#include "MyMath.h"

/**
* @file DistanceFog.h
* @brief 距離フォグファイル
*/

namespace MNE
{

	class DistanceFog
	{
	private:
		bool active_ = false;
		MyMath::Vector3D color_ = MyMath::Vector3D(1.0f, 1.0f, 1.0f);
		float start_ = 0.1f;
		float end_ = 5.0f;
		float fogFar_ = 30.0f;
		float fogNear_ = 0.1f;

		MyMath::Vector3D centerPos_;

	public:
	#pragma region Getter

		/**
		* @fn GetIsActive()
		* active_のGetter関数
		* @return active_の値
		*/
		bool GetIsActive() { return active_; }
		/**
		* @fn GetColor()
		* color_のGetter関数
		* @return color_の値
		*/
		const MyMath::Vector3D& GetColor() { return color_; }
		/**
		* @fn GetCenterPos()
		* centerPos_のGetter関数
		* @return centerPos_の値
		*/
		const MyMath::Vector3D& GetCenterPos() { return centerPos_; }
		/**
		* @fn GetStart()
		* start_のGetter関数
		* @return start_の値
		*/
		float GetStart() { return start_; }
		/**
		* @fn GetEnd()
		* end_のGetter関数
		* @return end_の値
		*/
		float GetEnd() { return end_; }
		/**
		* @fn GetFar()
		* fogFar_のGetter関数
		* @return fogFar_の値
		*/
		float GetFar() { return fogFar_; }
		/**
		* @fn GetNear()
		* fogNear_のGetter関数
		* @return fogNear_の値
		*/
		float GetNear() { return fogNear_; }

	#pragma endregion

	#pragma region Setter

		/**
		* @fn SetActive(bool)
		* active_の値を変更するための関数
		* @param active active_の変更後の値
		*/
		void SetActive(bool active) { active_ = active; }
		/**
		* @fn SetColor(const Vector3D&)
		* color_の値を変更するための関数
		* @param color color_の変更後の値
		*/
		void SetColor(const MyMath::Vector3D& color) { color_ = color; }
		/**
		* @fn SetCenterPos(const Vector3D&)
		* centerPos_の値を変更するための関数
		* @param centerPos centerPos_の変更後の値
		*/
		void SetCenterPos(const MyMath::Vector3D& centerPos) { centerPos_ = centerPos; }
		/**
		* @fn SetStart(float)
		* start_の値を変更するための関数
		* @param start start_の変更後の値
		*/
		void SetStart(float start) { start_ = start; }
		/**
		* @fn SetEnd(float)
		* end_の値を変更するための関数
		* @param end end_の変更後の値
		*/
		void SetEnd(float end) { end_ = end; }
		/**
		* @fn SetFar(float)
		* fogFar_の値を変更するための関数
		* @param fogFar fogFar_の変更後の値
		*/
		void SetFar(float fogFar) { fogFar_ = fogFar; }
		/**
		* @fn SetNear(float)
		* fogNear_の値を変更するための関数
		* @param fogNear fogNear_の変更後の値
		*/
		void SetNear(float fogNear) { fogNear_ = fogNear; }

	#pragma endregion
	};

}
