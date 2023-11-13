#pragma once
#include "UIData.h"

class ClearUI
{
public:
	void Initialize();
	void Update();
	void Draw();

private:
	UIData data_;
	bool isActive_ = false;

public:
	void Start();
	bool GetIsActive() { return isActive_; }
};

