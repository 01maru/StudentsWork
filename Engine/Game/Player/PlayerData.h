#pragma once
#include <cstdint>

/**
* @file PlayerData.h
* @brief プレイヤーのステータスやパラメーターを管理するためのファイル
*/

class PlayerData
{
protected:
	/**
	* @fn ImGuiHPUpdate()
	* HPに関するImGui更新処理関数
	*/
	void ImGuiHPUpdate();
	/**
	* @fn ImGuiMoveUpdate()
	* 移動に関するImGui更新処理関数
	*/
	void ImGuiMoveUpdate();
	/**
	* @fn ImGuiAvoidUpdate()
	* 回避に関するImGui更新処理関数
	*/
	void ImGuiAvoidUpdate();
	/**
	* @fn ImGuiAttackUpdate()
	* 攻撃に関するImGui更新処理関数
	*/
	void ImGuiAttackUpdate();
	/**
	* @fn ImGuiJumpUpdate()
	* ジャンプに関するImGui更新処理関数
	*/
	void ImGuiJumpUpdate();
	/**
	* @fn SaveData()
	* データをファイルに保存するための関数
	*/
	void SaveData();
	/**
	* @fn LoadData()
	* データをファイルから読み込むための関数
	*/
	void LoadData();

protected:
	//	HP
	int32_t maxHP_ = 100;

	//	Move
	float walkSpd_ = 0.15f;
	float runSpd_ = 0.2f;
	float jumpingDecSpd_ = 0.75f;

	//	Avoid
	float avoidMaxSpd_ = 0.3f;
	int32_t avoidDecTime_ = 60;
	int32_t avoidAccTime_ = 30;
	int32_t avoidCoolTime_ = 240;

	//	Attack
	int32_t slowATCoolTime_ = 240;
	int32_t bulletRate_ = 20;

	// Jump
	float fallAcc = -0.01f;
	float fallVYMin = -0.5f;
	float jumpFirstSpd_ = 0.2f;

public:
#pragma region Getter

	/**
	* @fn GetWalkSpd()
	* 歩きの最大速度取得関数
	* @return 歩きの最大速度
	*/
	float GetWalkSpd();
	/**
	* @fn GetRunSpd()
	* 走りの最大速度取得関数
	* @return 走りの最大速度
	*/
	float GetRunSpd();
	/**
	* @fn GetJumpingSpdDec()
	* ジャンプの減速スピード取得関数
	* @return ジャンプの減速スピード
	*/
	float GetJumpingSpdDec();
	/**
	* @fn GetAvoidAccTime()
	* 回避の加速時間取得関数
	* @return 回避の加速時間
	*/
	int32_t GetAvoidAccTime();
	/**
	* @fn GetAvoidDecTime()
	* 回避の減速時間取得関数
	* @return 回避の減速時間
	*/
	int32_t GetAvoidDecTime();
	/**
	* @fn GetBulletRate()
	* 弾の発射レート取得関数
	* @return 弾の発射レート
	*/
	int32_t GetBulletRate();

#pragma endregion
};

