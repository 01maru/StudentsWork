#pragma once
#include "IGameState.h"

class ClearUI :public IGameState
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
};
