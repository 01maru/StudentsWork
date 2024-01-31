#pragma once
#include "Object3D.h"
#include "PlayerMoveState.h"
#include "PlayerAttackState.h"
#include "CharacterHP.h"
#include "SkillCoolTime.h"

#include <memory>

#include "Bullet.h"
#include <list>
#include "CrossHair.h"
#include "UIData.h"
#include "PlayerData.h"

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
	void Initialize(MNE::IModel* model_);
	void Update();
	void ImGuiUpdate();
	void CollisionUpdate();
	void OnCollision(CollisionInfo& info) override;

	void DrawBullets();
	void DrawUI();

private:
	//	HPの表示や生存フラグ管理用
	CharacterHP hp_;

#pragma region MoveState

	std::unique_ptr<PlayerMoveState> moveState_;
	bool isRunning_ = false;
	bool isMoving_ = false;
	bool onGround_ = true;

#pragma endregion
	//	攻撃
	std::unique_ptr<PlayerAttackState> attackState_;

	FrameCounter rate_;

	//	照準
	CrossHair crossHair_;
	
	//	平面上のスピード
	float spd_;
	//	平面上の移動方向(モデルの正面ベクトルでもある)
	Vector3D moveVec_;
	//	１フレームでの上下の移動量(上が+)
	float moveY_ = -0.5f;

	//	プレイヤーの中心とモデルの位置のオフセット値
	Vector3D offset_;

#pragma region Skills

	PlayerSkill nBulletSprite_;
	bool avoiding_ = false;
	SkillCoolTime avoidCTSprite_;
	SkillCoolTime slowAtCTSprite_;

#pragma endregion

	int32_t animationTimer_ = 0;

	//	Load&Save
	float maxAngle_ = 90.0f;
	float minAngle_ = 20.0f;

	//	消したい
	bool isActive_ = true;
	bool gameOver_ = false;
	GameOverUI* pGameOverState_ = nullptr;
	std::list<std::unique_ptr<Bullet>> bullets_;

private:
	void StatusInitialize();
	void IsMovingUpdate();
	void InputUpdate();
	void CalcModelFront(const Vector2D& inputVec);
	/**
	* @fn SkillsUpdate()
	* スキルの更新処理関数
	*/
	void SkillsUpdate();
	void JumpUpdate();

	void ImGuiMenuUpdate();

	void HotLoadStatus();
public:

	void AddBullet(std::unique_ptr<Bullet>& bullet);
	void StartSlowAtCT();
	void StartAvoidCT();

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

#pragma endregion

#pragma region Setter

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

	/**
	* @fn SetUIInfo(MNE::UIData&)
	* UIの配置用関数
	* @param uiData UIの配置情報
	*/
	void SetUIInfo(MNE::UIData& uiData);

	void SetGameOverState(IGameState* gameOverState);

#pragma endregion
};

