#pragma once

/**
* @file PlayerMoveState.h
* @brief プレイヤーの移動に関するステートの基底クラスファイル
*/

#pragma region 前置宣言
class Player;
#pragma endregion

class PlayerMoveState
{
public:
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	virtual void Initialize() {};
	/**
	* @fn Update()
	* 更新処理関数
	*/
	virtual void Update() {};
	/**
	* @fn ImGuiUpdate()
	* ImGui更新処理関数
	*/
	virtual void ImGuiUpdate() {};

protected:
	//	親ポインタ
	static Player* sPlayer_;

public:
#pragma region Setter

	/**
	* @fn SetPlayer(Player*)
	* 親ポインタ設定用関数
	* @param player 親ポインタ
	*/
	static void SetPlayer(Player* player);

#pragma endregion
};

