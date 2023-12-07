#include "IGameState.h"

void IGameState::Start()
{
	isActive_ = true;
	data_.Initialize();
}

bool IGameState::GetIsActive()
{
	return isActive_;
}
