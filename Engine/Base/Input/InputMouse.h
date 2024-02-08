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
	class InputMouse
	{
	public:
		enum MouseButton {
			LeftClick = 0,
			RightClick,
			WheelClick,
			SideClick,
			ButtonNum,
		};

	public:
		//	デストラクタ
		~InputMouse();

		/**
		* @fn Initialize(IDirectInput8*)
		* 初期化用関数
		* @param directInput デバイス生成用ダイレクトインプットポインタ
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

	private:	//	変数
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		ComPtr<IDirectInputDevice8> mouse_;

		//	入力状況格納変数
		DIMOUSESTATE click_ = {};
		//	1フレーム前の入力状況格納変数
		DIMOUSESTATE prevClick_ = {};

		//	クライアント領域のカーソル座標(カーソルの左上)
		MyMath::Vector2D cursor_;
		//	カーソルを中央に固定するかどうか
		bool isLockCursor_ = false;
		//	カーソルの表示フラグ
		bool showCursor_ = false;
		//	入力されているか
		bool isActive_ = false;

	private:	//	関数
		/**
		* @fn InputInfoUpdate()
		* 入力の情報更新処理関数
		*/
		void InputInfoUpdate();
		/**
		* @fn LockCursorUpdate()
		* カーソル固定時の更新処理関数
		*/
		void LockCursorUpdate();
		/**
		* @fn UnLockCursorUpdate()
		* カーソル非固定時の更新処理関数
		*/
		void UnLockCursorUpdate();
		/**
		* @fn MouseIsActiveUpdate()
		* マウスに関するいずれかの入力がされているかのフラグ更新処理関数
		*/
		void MouseIsActiveUpdate();

		/**
		* @fn ImGuiUpdateCursor()
		* カーソルに関するImGui更新処理関数
		*/
		void ImGuiUpdateCursor();
		/**
		* @fn ImGuiUpdateClick()
		* クリックに関するImGui更新処理関数
		*/
		void ImGuiUpdateClick();

	public:
#pragma region Getter

		/**
		* @fn GetIsActive()
		* マウスに関するいずれかの入力がされているかのフラグ取得用関数
		* @return マウスに関するいずれかの入力がされているか
		*/
		bool GetIsActive();
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
		const MyMath::Vector2D& GetCursor();
		/**
		* @fn GetPrevCursor()
		* 1フレームでのカーソルの移動量を返す関数
		* @return 1フレームでのカーソルの移動量
		*/
		MyMath::Vector2D GetCursorMoveVec();
		/**
		* @fn GetPrevCursor()
		* 1フレームでのホイール回転量を返す関数
		* @return 1フレームでのホイール回転量
		*/
		int32_t GetWheel();

#pragma endregion

#pragma region Setter

		/**
		* @fn SetCursorWinCenter()
		* カーソルをウィンドウ中央に移動するための関数
		*/
		void SetCursorWinCenter();
		/**
		* @fn SetShowCursor(bool)
		* showCursor_の値を変更するための関数
		* @param showCursor showCursor_の変更後の値
		*/
		void SetShowCursor(bool showCursor);
		/**
		* @fn SetLockCursor(bool)
		* isLockCursor_の値を変更するための関数
		* @param lockCursor isLockCursor_の変更後の値
		*/
		void SetLockCursor(bool lockCursor);

		void SetCursorPosition(const MyMath::Vector2D& pos);

#pragma endregion
	};

}
