#pragma once
#include "OptionScene.h"

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

	void LoadResources();
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();

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

private:
	void IsActiveUpdate();
	void PauseInputUpdate(bool selectButton);
	void PauseUpdate(bool selectButton);
	void OptionUpdate(bool selectButton);

public:
#pragma region Getter

	bool GetIsActive();

#pragma endregion

#pragma region Setter

	void SetIsActive(bool isActive);

#pragma endregion
};

