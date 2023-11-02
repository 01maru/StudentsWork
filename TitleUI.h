#pragma once
#include "OptionScene.h"
#include "Sprite.h"

class TitleUI
{
public:
	/**
	* @fn Initialize(const std::string&)
	* 初期化関数
	* @param filename レベルデータのファイル名
	*/
	void Initialize();
	/**
	* @fn Finalize()
	* 終了処理関数
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
	UIData data_;
	//	オプションの配置データ
	OptionScene option_;
	//	カーソルのスプライト
	Sprite cursor_;
	FrameCounter selectCounter_;

private:
	void TitleInputUpdate(bool selectButton);
	void TitleUpdate(bool selectButton);
	void OptionUpdate(bool selectButton);
	void CursorUpdate();
public:
	void Start();
};

