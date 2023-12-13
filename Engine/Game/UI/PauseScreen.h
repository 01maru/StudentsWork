#pragma once
#include "OptionScene.h"
#include "SelectCursor.h"
#include "GameCamera.h"

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

	GameCamera* gameCamera_ = nullptr;

private:
	/**
	* @fn IsActiveUpdate()
	* ポーズ画面が実行中か切り替えるための更新処理関数
	*/
	void IsActiveUpdate();
	/**
	* @fn PauseInputUpdate(bool)
	* ポーズ画面での入力更新用関数
	* @param selectButton 選択中かどうか
	*/
	void PauseInputUpdate(bool selectButton);
	/**
	* @fn PauseUpdate(bool)
	* ポーズ画面の更新用関数
	* @param selectButton 選択中かどうか
	*/
	void PauseUpdate(bool selectButton);
	/**
	* @fn OptionUpdate(bool)
	* オプション画面の更新用関数
	* @param selectButton 選択中かどうか
	*/
	void OptionUpdate(bool selectButton);
	/**
	* @fn MouseCursorInit()
	* マウスカーソル固定の設定用関数
	*/
	void MouseCursorInit();

public:
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

	void SetGameCamera(GameCamera* gamecamera);

#pragma endregion
};

