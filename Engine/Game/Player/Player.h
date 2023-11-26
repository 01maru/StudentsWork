#pragma once
#include "Object3D.h"
#include "PlayerMoveState.h"
#include "PlayerAttackState.h"
#include "CharacterHP.h"
#include "AvoidCoolTimer.h"
#include <memory>

#include "Bullet.h"
#include <list>
#include "Sprite.h"

class Player :public Object3D
{
private:
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

	Sprite crossHair_;
	
	//	平面上のスピード
	float spd_;
	//	平面上の移動方向
	Vector3D moveVec_;
	//	１フレームでの上下の移動量(上が+)
	float moveY_;

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
	float jumpFirstSpd_ = 0.2f;

	float avoidMaxSpd_ = 0.3f;

private:
	void StatusInitialize();
	void IsMovingUpdate();
	void CalcModelFront();
	void CoolTimeUpdate();
	void JumpUpdate();

	void ImGuiMenuUpdate();

	void SavePlayerStatus();
public:
	void Initialize(IModel* model_);
	void Update();
	void ImGuiUpdate();
	void CollisionUpdate();
	void OnCollision(CollisionInfo& info) override;

	void DrawBullets();
	void DrawUI();

	void AddBullet(std::unique_ptr<Bullet>& bullet);
	void StartSlowAtCT();
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
	Vector3D GetFrontVec();
	bool GetRateCountIsActive();
	bool GetSlowAtIsActive();
	int32_t GetBulletRate();

#pragma endregion

#pragma region Setter

	void SetMoveState(std::unique_ptr<PlayerMoveState>& moveState);
	void SetAttackState(std::unique_ptr<PlayerAttackState>& attackState);
	void SetSpd(float spd);
	void SetIsAvoid(bool isAvoid);
	void SetIsRunning(bool isRunning);
	void SetBulletRate(int32_t rate);
	void StartRateCount();

#pragma endregion
};

