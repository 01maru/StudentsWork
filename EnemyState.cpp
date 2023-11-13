#include "EnemyState.h"
#include "Boss.h"

Boss* EnemyState::sBoss_ = nullptr;

void EnemyState::SetBoss(Boss* boss)
{
	sBoss_ = boss;
}
