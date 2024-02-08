#pragma once
#include "UIData.h"

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
	* @param dikSelectButton 入力されているか
	* @return オプションが閉じるタイミングだけTRUEを返す
	*/
	bool InputUpdate(bool dikSelectButton, int16_t inputV);
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
	//	実行中か
	bool isActive_ = false;
	//	UIの配置データ
	MNE::UIData data_;
	//	カーソルポインタ
	SelectCursor* cursor_ = nullptr;
	//	実行する前のカーソルの位置
	MyMath::Vector2D befCursorPos_;

	float inputSpd_ = 1.0f;

private:
	/**
	* @fn SensUpdate(int16_t)
	* 視点感度更新処理用関数
	* @param inputValue 数値変更用のキー入力値
	*/
	void SensUpdate(int16_t inputValue);
	/**
	* @fn VolumeUpdate(const std::string&, int16_t)
	* ボリューム更新処理用関数
	* @param objectName 変更するボリュームタイプの名前
	* @param inputValue 数値変更用のキー入力値
	*/
	void VolumeUpdate(const std::string& objectName, int16_t inputValue);

	void CloseOption();

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
	MyMath::Vector2D GetSelectPosition();
	/**
	* @fn GetSelecting()
	* 選択中かのGetter関数
	* @return 選択中かどうか
	*/
	bool GetSelecting();

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
	void SetCursorBackPos(const MyMath::Vector2D& pos);
	/**
	* @fn SetSelectCursor(SelectCursor*)
	* カーソルのポインター設定用関数
	* @param cursor カーソルのポインター
	*/
	void SetSelectCursor(SelectCursor* cursor);

	/**
	* @fn ResetSelectButton()
	* 選択中のボタンリセットする用関数
	*/
	void ResetSelectButton();
	/**
	* @fn ResetAnimation(bool)
	* アニメーションをリセットするための関数する用関数
	* @param startingAnimation 開始時か終了時どちらのアニメーションか設定
	*/
	void ResetAnimation(bool startingAnimation);

#pragma endregion
};

