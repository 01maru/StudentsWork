#pragma once
#include "OptionScene.h"
#include "SelectCursor.h"

/**
* @file TitleUI.h
* @brief TitleのUIを管理するファイル
*/

#pragma region 前置宣言

class TitleCamera;

#pragma endregion

class TitleUI
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize();
	/**
	* @fn Finalize()
	* 終了時に呼び出す関数
	*/
	void Finalize();
	/**
	* @fn LoadResources()
	* リソース読み込み処理関数
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
	//	タイトルの配置データ
	MNE::UIData titleData_;
	//	オプションの配置データ
	OptionScene option_;
	//	カーソルのスプライト
	SelectCursor cursor_;
	//	カメラのポインター
	TitleCamera* pCamera_ = nullptr;

	int16_t inputValue_ = 0;
	float inputSpd_ = 1.0f;

private:
	/**
	* @fn TitleInputUpdate(bool)
	* タイトル画面での入力更新用関数
	* @param dikSelectButton 選択中かどうか
	*/
	void TitleInputUpdate(bool dikSelectButton);
	/**
	* @fn TitleUpdate(bool)
	* タイトル画面の更新用関数
	* @param dikSelectButton 選択中かどうか
	*/
	void TitleUpdate(bool dikSelectButton);
	/**
	* @fn OptionUpdate(bool)
	* オプション画面の更新用関数
	* @param dikSelectButton 選択中かどうか
	*/
	void OptionUpdate(bool dikSelectButton);

	void InputValueUpdate();

public:
#pragma region Setter

	/**
	* @fn Start()
	* 出現演出開始用関数
	*/
	void Start();
	/**
	* @fn SetTitleCamera(TitleCamera*)
	* メインカメラセット関数(カメラ動かす用)
	* @param pCamera カメラのポインター
	*/
	void SetTitleCamera(TitleCamera* pCamera);

#pragma endregion
};

