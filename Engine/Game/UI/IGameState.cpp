#include "IGameState.h"

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

bool IGameState::GetIsActive()
{
	return isActive_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void IGameState::Start()
{
	isActive_ = true;
	data_.ResetAnimation(true);
}

void IGameState::Reset()
{
	isActive_ = false;
	data_.Reset();
}
