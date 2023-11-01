#pragma once
#include "UIButtonManager.h"
#include "FrameCounter.h"
#include "UIObject.h"
#include <map>

/**
* @file UIData.h
* @brief UIEditor用のUIData構造体ファイル
*/

class UIData
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize();
	/**
	* @fn Finalize()
	* 終了処理関数
	*/
	void Finalize();
	void InputUpdate();
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw();

protected:
	//	全体のFrameCounter
	std::unique_ptr<FrameCounter> count_;
	std::unique_ptr<UIButtonManager> buttonMan_;
	std::map<std::string, std::unique_ptr<UIObject>, std::less<>> obj_;

	std::map<std::string, uint16_t, std::less<>> tagName_;

public:
	/**
	* @fn LoadData(const std::string&)
	* レベルデータ読み込み用関数
	* @param filename レベルデータのファイル名
	*/
	void LoadData(const std::string& filename);

#pragma region Getter

	/**
	* @fn GetSelectName()
	* 選択中のボタンの名前のGetter関数
	* @return 選択中のボタンの名前
	*/
	const std::string& GetSelectName();
	/**
	* @fn GetSelectPosition()
	* 選択中の座標のGetter関数(カーソル用)
	* @return 選択中の座標
	*/
	Vector2D& GetSelectPosition();

	UIObject* GetUIObject(const std::string& name);

#pragma endregion
};

