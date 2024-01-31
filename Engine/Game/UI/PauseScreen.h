#pragma once
#include "OptionScene.h"
#include "SelectCursor.h"

/**
* @file PauseScreen.h
* @brief Pause画面の表示や設定を管理するファイル
*/

#pragma region 前置宣言

class GameCamera;

#pragma endregion

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
	* @return IsActiveの切り替わったタイミング取得(Trigger)
	*/
	bool Update();
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
	MNE::UIData pauseData_;
	//	オプションデータ
	OptionScene option_;
	//	ポーズ画面用のカーソル
	SelectCursor cursor_;
	//	カメラポインター
	GameCamera* gameCamera_ = nullptr;

	int16_t inputValue_ = 0;

private:
	/**
	* @fn IsActiveUpdate()
	* ポーズ画面が実行中か切り替えるための更新処理関数
	* @return IsActiveの切り替わったタイミング取得(Trigger)
	*/
	bool IsActiveUpdate();
	/**
	* @fn PauseInputUpdate(bool)
	* ポーズ画面での入力更新用関数
	* @param dikSelectButton 選択中かどうか
	*/
	void PauseInputUpdate(bool dikSelectButton);
	/**
	* @fn PauseUpdate(bool)
	* ポーズ画面の更新用関数
	* @param dikSelectButton 選択中かどうか
	*/
	void PauseUpdate(bool dikSelectButton);
	/**
	* @fn OptionUpdate(bool)
	* オプション画面の更新用関数
	* @param dikSelectButton 選択中かどうか
	*/
	void OptionUpdate(bool dikSelectButton);
	/**
	* @fn MouseCursorInit()
	* マウスカーソル固定の設定用関数
	*/
	void MouseCursorInit();

	void InputValueUpdate();

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
	/**
	* @fn SetGameCamera(GameCamera*)
	* 使用中のゲームカメラ設定用(カーソルロックやカメラの動きを止める用)
	* @param gamecamera ゲームカメラのポインター
	*/
	void SetGameCamera(GameCamera* gamecamera);

#pragma endregion
};

