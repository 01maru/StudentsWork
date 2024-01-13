#pragma once
#include "Framework.h"

/**
 * @file MyGame.h
 * @brief このゲーム独自の処理をまとめる用のファイル
 */

namespace MNE
{

class MyGame :public Framework
{
private:
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize() override;

	/**
	* @fn Update()
	* 更新処理をまとめた関数
	*/
	void Update() override;

	/**
	* @fn Draw()
	* 描画処理をまとめた関数
	*/
	void Draw() override;

	/**
	* @fn Finalize()
	* exe終了時に呼び出す関数
	*/
	void Finalize() override;
};

}

