#include "BossState.h"
#include "Boss.h"

Boss* BossState::sBoss_ = nullptr;

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void BossState::SetBoss(Boss* boss)
{
	sBoss_ = boss;
}
