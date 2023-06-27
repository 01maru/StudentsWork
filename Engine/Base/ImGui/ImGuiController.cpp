﻿#include "ImGuiController.h"
#include "ImGuiManager.h"

#include "FPSController.h"

ImGuiController* ImGuiController::GetInstance()
{
    static ImGuiController instance;
    return &instance;
}

void ImGuiController::Update()
{
	ImGuiManager* man = ImGuiManager::GetInstance();
	man->BeginWindow("Controller");
	man->LabelText("", "FPS:%4.2f", FPSController::GetInstance()->GetFPS());

	man->CheckBox("VolumeManager", activeVolumeManager_);
	man->CheckBox("InputManager", activeInputManager_);
	man->CheckBox("TextureManager", activeTextureManager_);
	man->CheckBox("UIManager", activeUIManager_);
	man->CheckBox("CameraManager", activeCameraManager_);
	man->CheckBox("LightManager", activeLightManager_);

	man->EndWindow();
}
