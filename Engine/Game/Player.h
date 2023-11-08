#pragma once
#include "Object3D.h"
#include "PlayerMoveState.h"
#include "CharacterHP.h"
#include "AvoidCoolTimer.h"
#include <memory>

class Player :public Object3D
{
private:
	CharacterHP hp_;

	//	state
	std::unique_ptr<PlayerMoveState> moveState_;
	bool isRunning_ = false;
	bool isMoving_ = false;
	bool onGround_ = true;

	//	攻撃ステート
	
	//	平面上のスピード
	float spd_;
	//	移動方向
	Vector3D moveVec_;
	//	１フレームでの上下の移動量
	float moveY_;

	Vector3D modelFront_;

	//	cooltimer
	AvoidCoolTimer avoid_;

	//	Load&Save
	float walkSpd_ = 1.0f;
	float runSpd_ = 2.0f;
	float jumpingSpdDec_ = 0.75f;
	int32_t avoidTime_ = 40;
	int32_t avoidCoolTime_ = 240;

	float fallAcc = -0.01f;
	float fallVYMin = -0.5f;
	float jumpFirstSpd_ = 0.2f;

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
	void OnCollision(const CollisionInfo& info) override;

#pragma region Getter

	bool GetOnGround();
	float GetWalkSpd();
	float GetRunSpd();
	float GetJumpingSpdDec();
	bool GetIsRunning();
	bool GetIsAvoid();
	bool GetIsMoving();
	int32_t GetAvoidTime();

#pragma endregion

#pragma region Setter

	void SetMoveState(std::unique_ptr<PlayerMoveState>& moveState);
	void SetSpd(float spd);
	void SetIsAvoid(bool isAvoid);

#pragma endregion
};

