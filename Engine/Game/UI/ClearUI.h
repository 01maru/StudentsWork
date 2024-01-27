#pragma once
#include "IGameState.h"

/**
* @file ClearUI.h
* @brief クリア時のスプライト演出管理ファイル
*/

class ClearUI :public IGameState
{
public:
	//	デストラクタ
	~ClearUI() {};
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize() override {};
	/**
	* @fn LoadResources()
	* リソース読み込み処理関数
	*/
	void LoadResources() override;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw() override;
};
