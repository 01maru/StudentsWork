#include "IGameState.h"

void IGameState::Start()
{
	isActive_ = true;
	data_.ResetAnimation(true);
}

bool IGameState::GetIsActive()
{
	return isActive_;
}
