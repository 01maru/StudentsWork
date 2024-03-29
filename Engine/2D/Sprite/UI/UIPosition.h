#pragma once
#include "UIComponent.h"
#include "Vector2D.h"

/**
* @file UIPosition.h
* @brief UIObjectで座標を使うためのファイル
*/

namespace MNE
{

	class UIPosition :public UIComponent
	{
	private:
		//	中心座標
		MyMath::Vector2D pos_;
		MyMath::Vector2D size_;

	public:
	#pragma region Getter

		/**
		* @fn GetPosition()
		* 座標のGetter関数
		* @return 座標
		*/
		MyMath::Vector2D& GetPosition();
		/**
		* @fn GetSize()
		* サイズのGetter関数
		* @return サイズ
		*/
		MyMath::Vector2D& GetSize();

	#pragma endregion

	#pragma region Setter

		/**
		* @fn SetPosition(const Vector2D&)
		* 座標の値を変更するための関数
		* @param pos 座標の変更値
		*/
		void SetPosition(const MyMath::Vector2D& pos);
		/**
		* @fn SetSize(const Vector2D&)
		* サイズの値を変更するための関数
		* @param size サイズの変更値
		*/
		void SetSize(const MyMath::Vector2D& size);

	#pragma endregion
	};

}
