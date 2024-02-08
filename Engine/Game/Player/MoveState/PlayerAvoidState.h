#pragma once
#include "PlayerMoveState.h"
#include "FrameCounter.h"
#include <cstdint>

class PlayerAvoidState: public PlayerMoveState
{
public:
	void Initialize() override;
	void Update() override;
	void ImGuiUpdate() override;

private:
	void SpdUpdate();
	void SetNextState();

	void DecSpdUpdate();
	void AccSpdUpdate();
private:
	MNE::FrameCounter timer_;
	int32_t decTimer_;
	bool acceleration_;

	float startSpd_;
	float maxSpd_;
	float endSpd_;
};

