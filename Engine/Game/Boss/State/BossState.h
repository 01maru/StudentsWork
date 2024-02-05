#pragma once

/**
* @file BossState.h
* @brief 継承用ボスのステートファイル
*/

#pragma region 前置宣言
class Boss;
#pragma endregion

class BossState
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

protected:
	//	親ポインタ
	static Boss* sBoss_;

public:
#pragma region Setter

	/**
	* @fn SetBoss(Boss*)
	* 親ポインター設定用関数
	* @param boss 親のポインター
	*/
	static void SetBoss(Boss* boss);

#pragma endregion
};

