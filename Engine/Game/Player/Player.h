#pragma once
#include "Object3D.h"
#include "PlayerMoveState.h"
#include "PlayerAttackState.h"
#include "CharacterHP.h"
#include "AvoidCoolTimer.h"
#include <memory>

#include "Bullet.h"
#include <list>
#include "CrossHair.h"

class IGameState;

class Player :public MNE::Object3D
{
private:
	bool isActive_ = true;
	bool gameOver_ = false;
	CharacterHP hp_;

	//	state
	std::unique_ptr<PlayerMoveState> moveState_;
	bool isRunning_ = false;
	bool isMoving_ = false;
	bool onGround_ = true;
	//	攻撃
	std::unique_ptr<PlayerAttackState> attackState_;

	std::list<std::unique_ptr<Bullet>> bullets_;
	FrameCounter rate_;

	CrossHair crossHair_;
	
	//	平面上のスピード
	float spd_;
	//	平面上の移動方向
	Vector3D moveVec_;
	//	１フレームでの上下の移動量(上が+)
	float moveY_ = -0.5f;

	Vector3D modelFront_;

	//	cooltimer
	AvoidCoolTimer avoidCT_;
	CoolTime slowAtCT_;

	//	Load&Save
	float walkSpd_ = 0.15f;
	float runSpd_ = 0.2f;
	float jumpingSpdDec_ = 0.75f;
	int32_t avoidDecTime_ = 70;
	int32_t avoidAccTime_ = 50;
	int32_t avoidCoolTime_ = 240;
	int32_t slowATCoolTime_ = 240;
	int32_t bulletRate_ = 20;

	float fallAcc = -0.01f;
	float fallVYMin = -0.5f;
	float jumpFirstSpd_ = 0.05f;

	float avoidMaxSpd_ = 0.3f;

	IGameState* pGameOverState_ = nullptr;

	int16_t animationIdx_ = 0;
	int32_t animationTimer_ = 0;

private:
	void StatusInitialize();
	void IsMovingUpdate();
	void CalcModelFront();
	void CoolTimeUpdate();
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

	void DecHP(int32_t damage);

#pragma region Getter

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
	void SetAnimationIdx(int16_t idx) { 
		GetAnimation()->SetAnimationIdx(idx);
		animationIdx_ = idx; }
	void SetAnimationTimer(int32_t timer) { animationTimer_ = timer; }
	void SetMoveState(std::unique_ptr<PlayerMoveState>& moveState);
	void SetAttackState(std::unique_ptr<PlayerAttackState>& attackState);
	void SetSpd(float spd);
	void SetIsAvoid(bool isAvoid);
	void SetIsRunning(bool isRunning);
	void SetBulletRate(int32_t rate);
	void StartRateCount();
	void SetCrossHairSprite(const MNE::Sprite& sprite);
	void SetHPBarSprite(const MNE::Sprite& sprite);
	void SetAvoidCoolSprite(const MNE::Sprite& sprite, const MNE::Sprite& text);
	void SetSlowAtCoolSprite(const MNE::Sprite& sprite, const MNE::Sprite& text);

	void SetGameOverState(IGameState* gameOverState);

#pragma endregion
};

