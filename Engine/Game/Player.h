#pragma once
#include "Object3D.h"

class InputManager;
class InputKeyboard;
class InputJoypad;

class Player :public Object3D
{
private:
	//	spd
	float spd_;
	const float MAX_SPD = 0.1f;

	//	jump
	bool onGround_ = true;
	Vector3D fallVec_;

	//	status
	bool isAlive_ = true;
	int32_t hp_;
	const int32_t MAX_HP = 100;
	float stamina_;
	int32_t staminaHealTimer_ = 0;
	const int MAX_STAMINA = 100;
	const int STAMINA_HEAL_TIME = 150;
	const int JUMP_STAMINA = 10;
	const int AVOID_STAMINA = 20;
	const float STAMINA_HEAL = 0.5f;

	//	‰ñ”ð
	Vector2D modelFrontVec_;
	bool isAvoid_ = false;
	int32_t avoidTimer_ = 0;
	const int32_t INVINCIBLE_TIME = 40;
	const float INVINCIBLE_SPD = 0.15f;

private:
	void StaminaUpdate();
	void AvoidUpdate(InputManager* input);
	Vector3D CalcMoveVec(InputKeyboard* keyboard, InputJoypad* pad, ICamera* camera);
	void CalcFallSpd(InputManager* input);
public:
	void Initialize(IModel* model_);
	void Update();
	void CollisionUpdate();
	void OnCollision(const CollisionInfo& info) override;

	//	Getter
	int32_t GetHP() { return hp_; }
	float GetStamina() { return stamina_; }
};

