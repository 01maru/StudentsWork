#pragma once
#include "UIComponent.h"
#include <cstdint>
#include <string>

/**
* @file UIButton.h
* @brief ボタン切り替えに必要な情報をまとめたButtonファイル
*/

#pragma region 前置宣言

class UIPosition;
class Vector2D;

#pragma endregion

class UIButton :public UIComponent
{
private:
	//	ボタンの名前
	std::string name_;

	//	次のボタンポインター
	UIButton* pNext_ = nullptr;
	//	前のボタンポインター
	UIButton* pPrev_ = nullptr;

public:
	
#pragma region Getter

	/**
	* @fn GetPosition()
	* UIPositionの座標のGetter関数
	* @return UIPositionの座標
	*/
	Vector2D& GetPosition();
	/**
	* @fn GetButtonName()
	* name_のGetter関数
	* @return このボタンの名前
	*/
	const std::string& GetButtonName();
	/**
	* @fn GetNextButton()
	* pNext_のGetter関数
	* @return pNext_の値
	*/
	UIButton* GetNextButton();
	/**
	* @fn GetPrevButton()
	* pPrev_のGetter関数
	* @return pPrev_の値
	*/
	UIButton* GetPrevButton();

#pragma endregion

#pragma region Setter

	/**
	* @fn SetName(const std::string&)
	* name_の値を変更するための関数
	* @param name name_の変更後の値
	*/
	void SetName(const std::string& name);
	/**
	* @fn SetNextButton(UIButton*)
	* pNext_の値を変更するための関数
	* @param pNext pNext_の変更後の値
	*/
	void SetNextButton(UIButton* pNext);
	/**
	* @fn SetPrevButton(UIButton*)
	* pPrev_の値を変更するための関数
	* @param pPrev pPrev_の変更後の値
	*/
	void SetPrevButton(UIButton* pPrev);

#pragma endregion
};

