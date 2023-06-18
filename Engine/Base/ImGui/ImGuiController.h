﻿#pragma once

class ImGuiController
{
private:
	bool activeVolumeManager_ = false;
	bool activeInputManager_ = false;

private:	//	関数
	ImGuiController() {};
	~ImGuiController() {};

public:
	static ImGuiController* GetInstance();
	ImGuiController(const ImGuiController& obj) = delete;
	ImGuiController& operator=(const ImGuiController& obj) = delete;

	void Update();

	//	Getter
	bool GetActiveVolumeManager() { return activeVolumeManager_; }
	bool GetActiveInputManager() { return activeInputManager_; }
};

