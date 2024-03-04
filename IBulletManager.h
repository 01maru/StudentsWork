#pragma once
#include "IBullet.h"
#include <list>

/**
* @file IBulletManager.h
* @brief 継承用弾管理クラスファイル
*/

class IBulletManager
{
public:
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();
	/**
	* @fn CollisionUpdate()
	* 当たり判定更新処理関数
	*/
	void CollisionUpdate();
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

protected:
	//	弾リスト
	std::list<std::unique_ptr<IBullet>> bullets_;
	//	弾と当たり判定を取る属性
	uint16_t attribute_;

public:
	/**
	* @fn SetAttribute(uint16_t)
	* 属性設定用関数
	* @param attribute 設定する属性
	*/
	void SetAttribute(uint16_t attribute);
};

