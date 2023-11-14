#pragma once
#include "SliderSprite.h"
#include "UIData.h"
#include "XAudioManager.h"

/**
* @file OptionScene.h
* @brief Optionの表示や設定を管理するファイル
*/

#pragma region 前置宣言

class SelectCursor;

#pragma endregion

class OptionScene
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize();
	/**
	* @fn LoadResources(const std::string&)
	* リソース読み込み用関数
	* @param filename レベルデータのファイル名
	*/
	void LoadResources(const std::string& filename);
	/**
	* @fn InputUpdate(bool)
	* 入力更新処理関数
	* @param selectButton 入力されているか
	*/
	void InputUpdate(bool selectButton);
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
	/**
	* @fn ResetSelectButton()
	* 選択中のボタンリセットする用関数
	*/
	void ResetSelectButton();

private:
	//	実行中か
	bool isActive_ = false;
	//	UIの配置データ
	UIData data_;
	//	カーソル
	SelectCursor* cursor_ = nullptr;
	//	実行する前のカーソルの位置
	Vector2D backPos_;

	//	光度
private:
	/**
	* @fn SensUpdate()
	* 視点感度更新処理用関数
	*/
	void SensUpdate(int16_t inputValue);
	/**
	* @fn VolumeUpdate()
	* ボリューム更新処理用関数
	*/
	void VolumeUpdate(const std::string& objectName, XAudioManager::SoundType type, int16_t inputValue);

public:
#pragma region Getter

	/**
	* @fn GetIsActive()
	* isActive_のGetter関数
	* @return isActive_の値
	*/
	bool GetIsActive();
	/**
	* @fn GetSelectPosition()
	* 選択中のオブジェクトの中心座標取得用関数
	* @return 選択中のオブジェクトの中心座標
	*/
	Vector2D& GetSelectPosition();
	Vector2D GetSelectScale();

#pragma endregion

#pragma region Setter

	/**
	* @fn SetIsActive(bool)
	* isActive_変更用関数
	* @param isActive isActive変更後の値
	*/
	void SetIsActive(bool active);
	/**
	* @fn SetCursorBackPos(const Vector2D&)
	* 実行する前のカーソルの位置設定用関数
	* @param pos 実行する前のカーソルの位置
	*/
	void SetCursorBackPos(const Vector2D& pos);
	/**
	* @fn SetSelectCursor(SelectCursor*)
	* カーソルのポインター設定用関数
	* @param cursor カーソルのポインター
	*/
	void SetSelectCursor(SelectCursor* cursor);

#pragma endregion
};

