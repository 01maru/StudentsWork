#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <wrl.h>
#include <stdint.h>

/**
* @file InputKeyboard.h
* @brief キーボードの入力処理をまとめたファイル
*/

namespace MNE
{

	class InputKeyboard
	{
	private:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		ComPtr<IDirectInputDevice8> keyboard_ = nullptr;

		int8_t key_[256] = {};
		int8_t prev_[256] = {};

	public:		//	関数
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

	#pragma region Getter

		/**
		* @fn GetKey(int)
		* キーが押されているか判定用関数
		* @param key 確認したい入力キーの設定
		* @return 押されているかどうか
		*/
		bool GetKey(int key);
		/**
		* @fn GetTrigger(int)
		* キーが押された瞬間の判定用関数
		* @param key 確認したい入力キーの設定
		* @return 押された瞬間ならtrueを返す
		*/
		bool GetTrigger(int key);
		/**
		* @fn GetRelease(int)
		* キーが離された瞬間の判定用関数
		* @param key 確認したい入力キーの設定
		* @return 離された瞬間ならtrueを返す
		*/
		bool GetRelease(int key);

	#pragma endregion
	};

}
