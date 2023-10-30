#pragma once
#include "UIData.h"


class GameOverUI
{
private:
	GameOverUI() {};
	~GameOverUI() {};

public:
	static GameOverUI* GetInstance();
	GameOverUI(const GameOverUI& obj) = delete;
	GameOverUI& operator=(const GameOverUI& obj) = delete;

	void Initialize();
	void Update();
	void Draw();

private:
	UIData data_;
	bool isActive_ = false;

public:
	void Start();
	//void LoadData();
};

