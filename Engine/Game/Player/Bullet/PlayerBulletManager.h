#pragma once
#include "Bullet.h"
#include "BulletInfo.h"
#include <list>

/**
* @file PlayerBulletManager.h
* @brief プレイヤーの弾丸をまとめて管理するファイル
*/

class PlayerBulletManager
{
public:
	/**
	* @fn LoadResources()
	* リソース読み込み処理関数
	*/
	void LoadResources();
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update(std::list<BulletInfo>& bullets);
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
	//	弾リスト
	std::list<std::unique_ptr<Bullet>> bullets_;
};

