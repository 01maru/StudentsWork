#pragma once
#include <dinput.h>
#include <wrl.h>
#include <stdint.h>
#include "Vector2D.h"

/**
* @file InputMouse.h
* @brief マウスの入力処理をまとめたファイル
*/

namespace MNE
{
	class ImGuiManager;

	class InputMouse
	{
	public:
		enum MouseButton {
			LeftClick = 0,
			RightClick,
			WheelClick,
			SideClick,
		};
	private:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		ComPtr<IDirectInputDevice8> mouse_;

		DIMOUSESTATE click_ = {};
		DIMOUSESTATE prevClick_ = {};

		Vector2D cursor_;
		Vector2D prevCursor_;
		Vector2D cursorMoveLen_;
		bool isLockCursor_ = false;
		bool showCursor_ = true;

	private:	//	関数
		//	入力の情報設定
		void SetInputInfo();
		void LockCursor();
		void UnLockCursor();

		void ImGuiUpdateCursor(MNE::ImGuiManager* imgui);
		void ImGuiUpdateClick(MNE::ImGuiManager* imgui);
	public:
		~InputMouse();

		/**
		* @fn Initialize()
		* 初期化用関数
		*/
		void Initialize(IDirectInput8* directInput);
		/**
		* @fn Update()
		* 更新処理関数
		*/
		void Update();
		/**
		* @fn Update()
		* ImGui更新処理関数
		*/
		void ImGuiUpdate();

	#pragma region Getter

		/**
		* @fn GetClick(MouseButton)
		* MouseButtonが押されているか判定用関数
		* @param type 確認したいMouseButtonの種類設定
		* @return 押されているかどうか
		*/
		bool GetClick(MouseButton type);
		/**
		* @fn GetClickTrigger(MouseButton)
		* MouseButton押された瞬間の判定用関数
		* @param type 確認したいMouseButtonの種類設定
		* @return 押された瞬間ならtrueを返す
		*/
		bool GetClickTrigger(MouseButton type);
		/**
		* @fn GetClickRelease(MouseButton)
		* MouseButton離された瞬間の判定用関数
		* @param type 確認したいMouseButtonの種類設定
		* @return 離された瞬間ならtrueを返す
		*/
		bool GetClickRelease(MouseButton type);
		/**
		* @fn GetCursor()
		* cursor_の値を返す関数
		* @return cursor_の値
		*/
		const Vector2D& GetCursor() { return cursor_; }
		/**
		* @fn GetPrevCursor()
		* prevCursor_の値を返す関数
		* @return prevCursor_の値
		*/
		const Vector2D& GetPrevCursor() { return prevCursor_; }
		/**
		* @fn GetPrevCursor()
		* 1フレームでのカーソルの移動量を返す関数
		* @return 1フレームでのカーソルの移動量
		*/
		const Vector2D& GetMoveCursor() { return cursorMoveLen_; }
		/**
		* @fn GetPrevCursor()
		* 1フレームでのホイール回転量を返す関数
		* @return 1フレームでのホイール回転量
		*/
		int32_t GetWheel() { return click_.lZ; }

	#pragma endregion

	#pragma region Setter

		/**
		* @fn SetShowCursor(bool)
		* showCursor_の値を変更するための関数
		* @param showCursor showCursor_の変更後の値
		*/
		void SetShowCursor(bool showCursor) { showCursor_ = showCursor; }
		/**
		* @fn SetLockCursor(bool)
		* isLockCursor_の値を変更するための関数
		* @param isLockCursor isLockCursor_の変更後の値
		*/
		void SetLockCursor(bool lockCursor) { isLockCursor_ = lockCursor; }

	#pragma endregion
	};

}
