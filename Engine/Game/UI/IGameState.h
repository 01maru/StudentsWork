#pragma once
#include "UIData.h"

class IGameState
{
public:
	virtual ~IGameState() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	UIData data_;
	bool isActive_ = false;

public:
	virtual void Start();
	bool GetIsActive();
};

