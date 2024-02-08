#pragma once
#include "Vector2D.h"
#include <cstdint>
#include <string>

/**
* @file UIButtonManager.h
* @brief 選択中のUIButtonを切り替えるためのファイル
*/

namespace MNE
{
#pragma region 前置宣言

	class UIButton;

#pragma endregion

	class UIButtonManager
	{
	public:
		/**
		* @fn Update()
		* 更新処理関数
		*/
		void Update(int16_t inputValue);

	private:
		//	選択中のButton
		UIButton* selectButton_ = nullptr;

	public:

	#pragma region Getter

		/**
		* @fn GetSelectObjName()
		* 選択中のボタンの名前のGetter関数
		* @return 選択中のボタンの名前
		*/
		const std::string& GetSelectObjName();
		/**
		* @fn GetSelectPos()
		* 選択中の座標のGetter関数(カーソル用)
		* @return 選択中の座標
		*/
		MyMath::Vector2D& GetSelectPos();
		/**
		* @fn GetSelectSize()
		* 選択中のボタンサイズのGetter関数(カーソルアニメーション用)
		* @return 選択中のボタンサイズ
		*/
		MyMath::Vector2D& GetSelectSize();

	#pragma endregion

	#pragma region Setter

		/**
		* @fn SetSelectButton(UIButton*)
		* 選択中のボタン設定用関数
		* @param button 選択中のボタンポインター
		*/
		void SetSelectButton(UIButton* button);

	#pragma endregion
	};

}
