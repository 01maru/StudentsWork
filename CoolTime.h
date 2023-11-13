#pragma once
#include "FrameCounter.h"

class CoolTime
{
public:
	void Initialize();

	virtual void Update();

	void StartCount();
protected:
	bool isActive_ = false;
	FrameCounter coolTimer_;

public:
	bool GetIsActive();
#pragma region Setter

	void SetMaxTime(int32_t time);

#pragma endregion
};

