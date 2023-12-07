#pragma once
#include "IGameState.h"
#include "FrameCounter.h"

class GameOverCamera;

class GameOverUI :public IGameState
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

	void Start() override;

private:
	FrameCounter fadeCounter_;
	GameOverCamera* pCamera_ = nullptr;
};
