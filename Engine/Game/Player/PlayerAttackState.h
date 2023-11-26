#pragma once
#include <cstdint>
#include <memory>

class Player;

class PlayerAttackState
{
public:
	enum AttackState
	{
		NoAttack,
		NormalAttack,
		SlowAttack,
	};
public:
	virtual void Initialize() {};
	virtual void Update() {};

protected:
	static Player* sPlayer_;

protected:
	std::unique_ptr<PlayerAttackState> SetNextState(int32_t nowState);

public:
	static void SetPlayer(Player* player);
};
