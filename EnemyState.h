#pragma once

class Boss;

class EnemyState
{
public:
	virtual void Initialize() {};
	virtual void Update() {};

protected:
	static Boss* sBoss_;

public:
	static void SetBoss(Boss* boss);
};

