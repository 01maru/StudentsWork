#pragma once
#include "ICamera.h"
#include <memory>
#include <string>

class CameraManager
{
private:
	bool isDebug_ = false;

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
	void Finalize();
	void Update();
	void ImGuiUpdate();

	//	Getter
	ICamera* GetCamera();

	//	Setter
	void SetMainCamera(std::unique_ptr<ICamera> camera) { mainCamera_ = std::move(camera); }
};

