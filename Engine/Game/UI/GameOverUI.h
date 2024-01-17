#pragma once
#include "IGameState.h"
#include "SelectCursor.h"

#pragma region 前置宣言

class GameOverCamera;

#pragma endregion

/**
* @file GameOverUI.h
* @brief ゲームオーバー時の動き管理ファイル
*/

class GameOverUI :public IGameState
{
public:
	~GameOverUI() {};
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize() override;
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

private:
	GameOverCamera* pCamera_ = nullptr;
	//	カーソルのスプライト
	SelectCursor cursor_;

	bool activeAnime_ = false;

private:
	void InputUpdate();

public:
#pragma region Setter

	void Start() override;

#pragma endregion
};
