#pragma once

class Player;

class PlayerMoveState
{
public:
	virtual void Initialize() {};
	virtual void Update() {};

protected:
	static Player* sPlayer_;

public:
	static void SetPlayer(Player* player);
};

