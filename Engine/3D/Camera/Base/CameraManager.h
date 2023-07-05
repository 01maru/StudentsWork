#pragma once
#include "ICamera.h"
#include <memory>
#include <string>

class CameraManager
{
private:
	bool isDebug_ = true;

	std::unique_ptr<ICamera> mainCamera_;
	std::unique_ptr<ICamera> debugCamera_;
	std::unique_ptr<ICamera> lightCamera_;

private:
	CameraManager() {};
	~CameraManager() {};

	void SetDebugCameraPosToMain();
	void ImGuiCameraInfo(ICamera* camera, const std::string& name);
public:
	static CameraManager* GetInstance();
	CameraManager(const CameraManager& obj) = delete;
	CameraManager& operator=(const CameraManager& obj) = delete;

	void Initialize();
	void Update();
	void ImGuiUpdate();

	//	Getter
	ICamera* GetCamera();
	ICamera* GetLightCamera();
	ICamera* GetMainCamera();
	ICamera* GetDebugCamera();

	//	Setter
	void SetMainCamera(std::unique_ptr<ICamera> camera) { mainCamera_ = std::move(camera); }
	void SetLightCamera(std::unique_ptr<ICamera> camera) { lightCamera_ = std::move(camera); }
};

