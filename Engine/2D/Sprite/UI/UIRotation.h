#pragma once
#include "UIComponent.h"

/**
* @file UIRotation.h
* @brief UISpriteを回転させるためのファイル
*/

#pragma region 前置宣言

class UISprite;

#pragma endregion

class UIRotation :public UIComponent
{
public:
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;

private:
	float spd_ = 0.0f;

	UISprite* pSprites_ = nullptr;

public:
#pragma region Getter

	/**
	* @fn GetRotSpd()
	* スプライトの回転速度のGetter関数
	* @return スプライトの回転速度
	*/
	float GetRotSpd();
	
#pragma endregion

#pragma region Setter

	/**
	* @fn SetRotSpd(float)
	* 速度の変更用の関数
	* @param spd spd_の変更値
	*/
	void SetRotSpd(float spd);

#pragma endregion
};

