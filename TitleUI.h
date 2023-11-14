#pragma once
#include "OptionScene.h"
#include "SelectCursor.h"

/**
* @file TitleUI.h
* @brief TitleのUIを管理するファイル
*/

class TitleUI
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize();
	///**
	//* @fn Finalize()
	//* 終了処理関数
	//*/
	//void Finalize() {};
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
	UIData data_;
	//	オプションの配置データ
	OptionScene option_;
	//	カーソルのスプライト
	SelectCursor cursor_;

private:
	void TitleInputUpdate(bool selectButton);
	void TitleUpdate(bool selectButton);
	void OptionUpdate(bool selectButton);
public:
	/**
	* @fn Start()
	* 出現演出開始用関数
	*/
	void Start();
};

