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

private:
	//	ポーズ中か
	bool isActive_ = false;
	//	ポーズ画面のUI情報
	UIData data_;
	//	オプションデータ
	OptionScene option_;

private:
	void IsActiveUpdate();
	void PauseUpdate();

public:
#pragma region Getter

	bool GetIsActive();

#pragma endregion

#pragma region Setter

	void SetIsActive(bool isActive);

#pragma endregion
};

