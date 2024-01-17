#pragma once
#include "UIData.h"

/**
* @file IGameState.h
* @brief ゲームオーバーかクリアのUI表示継承用ファイル
*/

class IGameState
{
public:
	virtual ~IGameState() = default;
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	virtual void Initialize() = 0;
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
	MNE::UIData data_;
	bool isActive_ = false;

public:
	virtual void Start();
	bool GetIsActive();
};

