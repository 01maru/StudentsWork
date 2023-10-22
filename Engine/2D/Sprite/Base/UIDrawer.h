#pragma once
#include "UIData.h"

/**
* @file UIDrawer.h
* @brief UIEditor用で作成したレベルデータを表示するためのファイル
*/

class UIDrawer :public UIData
{
private:
	std::string activeTagName_;

public:
	void Initialize();
	/**
	* @fn Update(int16_t)
	* 更新処理関数
	* @param inputValue 入力値(-1～1)
	*/
	void Update(int16_t inputValue);
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw();

	/**
	* @fn LoadSprites(const std::string&)
	* レベルデータ読み込み用関数
	* @param filename レベルデータのファイル名
	*/
	void LoadSprites(const std::string& filename);

#pragma region Getter

	const Vector2D& GetSelectPosition() { return buttonMan_.GetSelectPosition(); }
	const std::string& GetActiveTagName() { return activeTagName_; }
	const std::string& GetActiveButtonName() { return buttonMan_.GetSelectName(); }

#pragma endregion

	void SetUI(const std::string& uiName);
};

