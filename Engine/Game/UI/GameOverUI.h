#pragma once
#include "IGameState.h"
#include "SelectCursor.h"

/**
* @file GameOverUI.h
* @brief ゲームオーバー時の入力を管理ファイル
*/

#pragma region 前置宣言

class GameOverCamera;

#pragma endregion

class GameOverUI :public IGameState
{
public:
	//	デストラクタ
	~GameOverUI() {};
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize() override;
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

private:
	//	ゲームオーバー用カメラポインター
	GameOverCamera* pCamera_ = nullptr;
	//	カーソルのスプライト
	SelectCursor cursor_;
	//	アニメーションを開始するためのダーティーフラグ
	bool animeDirtyFlag_ = true;

private:
	/**
	* @fn InputUpdate()
	* 入力更新処理関数
	*/
	void InputUpdate();

public:
#pragma region Setter

	/**
	* @fn SetCameraPosData(const Vector3D&)
	* カメラの終了位置や制御点を設定するための関数
	* @param playerPos プレイヤーの位置(足元の座標)
	*/
	void SetCameraPosData(const Vector3D& playerPos);
	/**
	* @fn Start()
	* アニメーションを開始させる関数
	*/
	void Start() override;
	/**
	* @fn Reset()
	* アニメーションをリセットするための関数
	*/
	void Reset() override;

#pragma endregion
};
