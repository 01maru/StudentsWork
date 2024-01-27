#pragma once
#include "UIData.h"

/**
* @file IGameState.h
* @brief ゲームオーバーかクリアのUI表示継承用ファイル
*/

class IGameState
{
public:
	//	デストラクタ
	virtual ~IGameState() = default;
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	virtual void Initialize() = 0;
	/**
	* @fn LoadResources()
	* リソース読み込み処理関数
	*/
	virtual void LoadResources() = 0;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	virtual void Update() = 0;
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	virtual void Draw() = 0;

protected:
	//	実行中か
	bool isActive_ = false;
	//	配置情報
	MNE::UIData data_;

public:
#pragma region Getter

	/**
	* @fn GetIsActive()
	* 実行中かのフラグ取得用関数
	* @return 実行中かならTRUE
	*/
	bool GetIsActive();

#pragma endregion

#pragma region Setter

	/**
	* @fn Start()
	* アニメーションを開始させる関数
	*/
	virtual void Start();
	/**
	* @fn Reset()
	* アニメーションをリセットするための関数
	*/
	virtual void Reset();

#pragma endregion
};

