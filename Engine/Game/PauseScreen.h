#pragma once
#include "OptionScene.h"
#include "SelectCursor.h"

/**
* @file PauseScreen.h
* @brief Pause画面の表示や設定を管理するファイル
*/

class PauseScreen
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize();
	/**
	* @fn LoadResources()
	* リソース読み込み用関数
	*/
	void LoadResources();
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();
	/**
	* @fn ImGuiUpdate()
	* ImGui更新処理関数
	*/
	void ImGuiUpdate();
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw();

private:
	//	ポーズ中か
	bool isActive_ = false;
	//	ポーズ画面のUI情報
	UIData data_;
	//	オプションデータ
	OptionScene option_;
	//	ポーズ画面用のカーソル
	SelectCursor cursor_;

private:
	void IsActiveUpdate();
	void PauseInputUpdate(bool selectButton);
	void PauseUpdate(bool selectButton);
	void OptionUpdate(bool selectButton);

public:
	void MouseCursorInit();

#pragma region Getter

	/**
	* @fn GetIsActive()
	* isActive_のGetter関数
	* @return isActive_の値
	*/
	bool GetIsActive();

#pragma endregion

#pragma region Setter

	/**
	* @fn SetIsActive(bool)
	* isActive_変更用関数
	* @param isActive isActive変更後の値
	*/
	void SetIsActive(bool isActive);

#pragma endregion
};

