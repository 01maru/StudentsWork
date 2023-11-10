#pragma once
#include "CoolTime.h"

class AvoidCoolTimer :public CoolTime
{
public:
	void Update() override;
	void ImGuiUpdate();
private:
	void InputUpdate();

private:
	bool isAvoiding_ = false;

public:
	bool GetIsAvoiding();

	void SetIsAvoiding(bool isAvoiding);
};

