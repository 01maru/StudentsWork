#pragma once
#include "Object3D.h"
#include "PlayerMoveState.h"
#include "PlayerAttackState.h"
#include "CharacterHP.h"
#include "SkillCoolTime.h"
#include "CrossHair.h"
#include "UIData.h"
#include "PlayerData.h"
#include "BulletInfo.h"
#include <memory>

/**
* @file Player.h
* @brief プレイヤーの動きを管理するためのファイル
*/

#pragma region 前置宣言

class IGameState;
class GameOverUI;

#pragma endregion

class Player :public MNE::Object3D, public PlayerData
{
public:
	/**
	* @fn StatusInitialize()
	* ステータスの初期化関数
	*/
	void StatusInitialize();
	/**
	* @fn Initialize(MNE::IModel*)
	* 初期化関数
	* @param model 使用するモデル
	*/
	void Initialize(MNE::IModel* model);
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();
	/**
	* @fn ImGuiUpdate()
	* ImGui更新処理関数
	*/
	void ImGuiUpdate();

	void CollisionUpdate();
	void OnCollision(CollisionInfo& info) override;

	/**
	* @fn DrawUI()
	* UI描画処理関数
	*/
	void DrawUI();

private:
	//	HPの表示や生存フラグ管理用
	CharacterHP hp_;

	//	プレイヤーの中心とモデルの位置のオフセット値
	Vector3D offset_;

	int32_t animationTimer_ = 0;

#pragma region Move

	std::unique_ptr<PlayerMoveState> moveState_;
	bool isRunning_ = false;
	bool isMoving_ = false;
	bool onGround_ = true;

	//	平面上のスピード
	float spd_;
	//	平面上の移動方向(モデルの正面ベクトルでもある)
	Vector3D moveVec_;
	//	１フレームでの上下の移動量(上が+)
	float moveY_ = -0.5f;

#pragma endregion

#pragma region Attack

	//	攻撃
	std::unique_ptr<PlayerAttackState> attackState_;
	FrameCounter rate_;
	//	照準
	CrossHair crossHair_;
	std::list<BulletInfo> bullets_;

#pragma endregion

#pragma region Skills

	PlayerSkill nBulletSprite_;
	bool avoiding_ = false;
	SkillCoolTime avoidCTSprite_;
	SkillCoolTime slowAtCTSprite_;

#pragma endregion

	//	消したい
	bool isActive_ = true;
	bool gameOver_ = false;
	GameOverUI* pGameOverState_ = nullptr;

private:
	/**
	* @fn CalcMoveVec(const Vector2D&)
	* 入力とモデルの向きから補間した実際の移動方向ベクトルの計算処理関数
	* @param inputVec (x, y) = (左右, 前後)の入力ベクトル
	*/
	void CalcMoveVec(const Vector2D& inputVec);
	/**
	* @fn CalcModelFront(const Vector2D&)
	* 実際の移動方向ベクトルからモデルの向きを変更の計算処理関数
	* @param inputVec (x, y) = (左右, 前後)の入力ベクトル
	*/
	void CalcModelFront(const Vector2D& inputVec);
	/**
	* @fn IsMovingUpdate()
	* 移動関連の処理をまとめた更新処理関数
	*/
	void IsMovingUpdate();
	/**
	* @fn InputUpdate()
	* スライディングや走るときの入力更新処理関数
	*/
	void InputUpdate();
	/**
	* @fn SkillsUpdate()
	* スキルの更新処理関数
	*/
	void SkillsUpdate();
	/**
	* @fn JumpUpdate()
	* ジャンプ周りの更新処理関数
	*/
	void JumpUpdate();
	/**
	* @fn ImGuiMenuUpdate()
	* ImGuiのメニューの更新処理関数
	*/
	void ImGuiMenuUpdate();
	/**
	* @fn HotLoadStatus()
	* ステータスをホットリロードするための関数
	*/
	void HotLoadStatus();

public:

	void DecHP(int32_t damage);

#pragma region Getter

	Vector3D* GetPositionPtr() { return &mat_.trans_; }
	Vector3D GetCenterPos();
	bool GetOnGround();
	bool GetIsRunning();
	bool GetIsAvoid();
	bool GetIsMoving();
	float GetSpd();
	float GetAvoidMaxSpd();
	Vector3D GetBulletFront();
	bool GetRateCountIsActive();
	bool GetSlowAtIsActive();
	bool GetIsAlive();
	std::list<BulletInfo>& GetBullets();

#pragma endregion

#pragma region Setter

	/**
	* @fn AddBullet(BulletInfo&)
	* 追加する弾の情報をリストに加えるための関数
	* @param bullet 追加する弾の情報
	*/
	void AddBullet(BulletInfo& bullet);
	/**
	* @fn StartSlowAtCT()
	* スロー攻撃のクールタイムを開始にするための関数
	*/
	void StartSlowAtCT();
	/**
	* @fn StartAvoidCT()
	* 回避のクールタイムを開始するための関数
	*/
	void StartAvoidCT();

	/**
	* @fn SetUIInfo(MNE::UIData&)
	* UIの配置用関数
	* @param uiData UIの配置情報
	*/
	void SetUIInfo(MNE::UIData& uiData);

	void SetIsActive(bool isActive) { isActive_ = isActive; }
	void SetAnimationIdx(const std::string& name) { 
		GetAnimation()->SetAnimeName(name); }
	void SetAnimationTimer(int32_t timer) { animationTimer_ = timer; }
	void SetMoveState(std::unique_ptr<PlayerMoveState>& moveState);
	void SetAttackState(std::unique_ptr<PlayerAttackState>& attackState);
	void SetSpd(float spd);
	void SetIsAvoid(bool isAvoid);
	void SetIsRunning(bool isRunning);
	void SetBulletRate(int32_t rate);
	void StartRateCount();


	void SetGameOverState(IGameState* gameOverState);

#pragma endregion
};

