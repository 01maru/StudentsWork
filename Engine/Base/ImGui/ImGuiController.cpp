#include "ImGuiController.h"
#include "ImGuiManager.h"

#include "FPSController.h"

MNE::ImGuiController* MNE::ImGuiController::GetInstance()
{
    static ImGuiController instance;
    return &instance;
}

void MNE::ImGuiController::Update()
{
	ImGuiManager* man = ImGuiManager::GetInstance();
	man->BeginWindow("Controller");
	man->LabelText("", "FPS:%4.2f", FPSController::GetInstance()->GetFPS());

	man->CheckBox("VolumeManager", activeVolumeManager_);
	man->CheckBox("InputManager", activeInputManager_);
	man->CheckBox("TextureManager", activeTextureManager_);
	man->CheckBox("UIEditor", activeUIManager_);
	man->CheckBox("CameraManager", activeCameraManager_);
	man->CheckBox("LightManager", activeLightManager_);
	man->CheckBox("ParticleManager", activeParticleManager_);
	man->CheckBox("ModelManager", activeModelManager_);

	man->EndWindow();
}
