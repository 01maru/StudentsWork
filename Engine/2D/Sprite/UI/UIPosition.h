#pragma once
#include "UIComponent.h"
#include "Vector2D.h"

/**
* @file UIPosition.h
* @brief UIObjectで座標を使うためのファイル
*/

class UIPosition :public UIComponent
{
private:
	//	中心座標
	Vector2D pos_;

public:
#pragma region Getter

	/**
	* @fn GetPosition()
	* 座標のGetter関数
	* @return 座標
	*/
	Vector2D& GetPosition();

#pragma endregion

#pragma region Setter

	/**
	* @fn SetPosition(const Vector2D&)
	* 座標の値を変更するための関数
	* @param pos 座標の変更値
	*/
	void SetPosition(const Vector2D& pos);

#pragma endregion
};

