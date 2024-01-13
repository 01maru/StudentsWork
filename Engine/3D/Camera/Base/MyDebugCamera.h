#pragma once
#include "Vector2D.h"
#include "ICamera.h"

/**
* @file MyDebugCamera.h
* @brief デバッグカメラファイル
*/

namespace MNE
{

	class InputMouse;
	class InputKeyboard;

	class MyDebugCamera :public MNE::ICamera
	{
	private:
		const float MIN_EYE_TO_TARGET = 1.0f;

		MNE::InputMouse* mouse_ = nullptr;
		MNE::InputKeyboard* keyboard_ = nullptr;

		Vector2D rotValue_;
		float transSpd_ = 0.1f;
		float frontMoveSpd_ = 0.001f;
		float mouseMoveRate_ = 1000.0f;

		enum MoveMode {
			TranslationMove,
			RotationMove,
		};
		MoveMode mode_ = TranslationMove;

	private:
		void CalcDisEyeToTarget();
		void SetMoveMode(bool active);
		Vector3D CalcTransMove(bool active);
		void CalcRotMove(bool active);
		void SetPosition(const Vector3D& moveTarget);

		/**
		* @fn ImGuiInfo()
		* ImGuiに表示する情報をまとめる関数
		*/
		void ImGuiInfo() override;

	public:
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

}
