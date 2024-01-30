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

/**
* @file Player.h
* @brief プレイヤーの動きを管理するためのファイル
*/

#pragma region 前置宣言

class IGameState;
class GameOverUI;

#pragma endregion

class Player :public MNE::Object3D
{
private:
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

	CrossHair crossHair_;
	
	//	平面上のスピード
	float spd_;
	//	平面上の移動方向
	Vector3D moveVec_;
	//	１フレームでの上下の移動量(上が+)
	float moveY_ = -0.5f;

	Vector3D modelFront_;

	Vector3D offset_;

#pragma region Skills

	PlayerSkill nBulletSprite_;
	bool avoiding_ = false;
	SkillCoolTime avoidCTSprite_;
	SkillCoolTime slowAtCTSprite_;

#pragma endregion

	//	Load&Save
	int32_t maxHP_ = 100;
	float walkSpd_ = 0.15f;
	float runSpd_ = 0.2f;
	float jumpingSpdDec_ = 0.75f;
	int32_t avoidDecTime_ = 60;
	int32_t avoidAccTime_ = 30;
	int32_t avoidCoolTime_ = 240;
	int32_t slowATCoolTime_ = 240;
	int32_t bulletRate_ = 20;

	float fallAcc = -0.01f;
	float fallVYMin = -0.5f;
	float jumpFirstSpd_ = 0.2f;

	float avoidMaxSpd_ = 0.3f;

	int32_t animationTimer_ = 0;

	//	消したい
	bool isActive_ = true;
	bool gameOver_ = false;
	GameOverUI* pGameOverState_ = nullptr;
	std::list<std::unique_ptr<Bullet>> bullets_;

private:
	void StatusInitialize();
	void IsMovingUpdate();
	void CalcModelFront();
	/**
	* @fn SkillsUpdate()
	* スキルの更新処理関数
	*/
	void SkillsUpdate();
	void JumpUpdate();

	void ImGuiMenuUpdate();

	void SavePlayerStatus();
public:
	void Initialize(MNE::IModel* model_);
	void Update();
	void ImGuiUpdate();
	void CollisionUpdate();
	void OnCollision(CollisionInfo& info) override;

	void DrawBullets();
	void DrawUI();

	void AddBullet(std::unique_ptr<Bullet>& bullet);
	void StartSlowAtCT();
	void StartAvoidCT();

	void DecHP(int32_t damage);

#pragma region Getter

	Vector3D* GetPositionPtr() { return &mat_.trans_; }
	Vector3D GetCenterPos();
	bool GetOnGround();
	float GetWalkSpd();
	float GetRunSpd();
	float GetJumpingSpdDec();
	bool GetIsRunning();
	bool GetIsAvoid();
	bool GetIsMoving();
	int32_t GetAvoidAccTime();
	int32_t GetAvoidDecTime();
	float GetSpd();
	float GetAvoidMaxSpd();
	Vector3D GetBulletFront();
	bool GetRateCountIsActive();
	bool GetSlowAtIsActive();
	int32_t GetBulletRate();
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

