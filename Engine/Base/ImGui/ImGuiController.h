#pragma once

class ImGuiController
{
private:
	bool activeVolumeManager_ = false;
	bool activeInputManager_ = false;
	bool activeTextureManager_ = false;
	bool activeUIManager_ = false;
	bool activeCameraManager_ = false;
	bool activeLightManager_ = false;
	bool activeDoF_ = false;
	bool activeTask_ = false;
	bool activeParticleManager_ = false;

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
	bool GetActiveTextureManager() { return activeTextureManager_; }
	bool GetActiveUIManager() { return activeUIManager_; }
	bool GetActiveCameraManager() { return activeCameraManager_; }
	bool GetActiveLightManager() { return activeLightManager_; }
	bool GetActiveDof() { return activeDoF_; }
	bool GetActiveTask() { return activeTask_; }
	bool GetActiveParticleManager() { return activeParticleManager_; }
};

