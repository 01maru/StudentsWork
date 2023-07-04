#include "CameraManager.h"
#include "ImGuiController.h"
#include "ImGuiManager.h"

#include "MyDebugCamera.h"

CameraManager* CameraManager::GetInstance()
{
    static CameraManager instance;
    return &instance;
}

void CameraManager::Initialize()
{
	debugCamera_ = std::make_unique<MyDebugCamera>();
	debugCamera_->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
}

void CameraManager::Finalize()
{
	mainCamera_.release();
	debugCamera_.release();
	lightCamera_.release();
}

void CameraManager::Update()
{
	if(isDebug_)				debugCamera_->Update();
	if (mainCamera_ != nullptr) mainCamera_->Update();
	if (lightCamera_ != nullptr) lightCamera_->Update();
}

void CameraManager::SetDebugCameraPosToMain()
{
	//	mainCameraがセットされていなかったら
	if (mainCamera_ == nullptr) return;

	debugCamera_->SetEye(mainCamera_->GetEye());
	debugCamera_->SetTarget(mainCamera_->GetTarget());
	debugCamera_->SetUp(mainCamera_->GetUp());
}

void CameraManager::ImGuiCameraInfo(ICamera* camera, const std::string& name)
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	if (imguiMan->CollapsingHeader(name))
	{
		if (camera == nullptr) return;
		
		Vector3D vec = camera->GetEye();
		imguiMan->SetSliderFloat3("Eye", vec);
		camera->SetEye(vec);

		vec = camera->GetTarget();
		imguiMan->SetSliderFloat3("Target", vec);
		camera->SetTarget(vec);

		vec = camera->GetUp();
		imguiMan->SetSliderFloat3("UP", vec);
		camera->SetUp(vec);
	}

	imguiMan->PopID();
}

void CameraManager::ImGuiUpdate()
{
    //  Activeじゃなかったら
    if (!ImGuiController::GetInstance()->GetActiveCameraManager()) return;

	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("CameraManager", true);

	imguiMan->CheckBox("IsDebug", isDebug_);

	//if (imguiMan->SetButton("Pos : MainCamera = DebugCamera")) SetDebugCameraPosToMain();
	int id = 0;

	imguiMan->PushID(id++);

	ImGuiCameraInfo(mainCamera_.get(), "MainCamera");

	imguiMan->PushID(id++);

	ImGuiCameraInfo(debugCamera_.get(), "DebugCamera");

	imguiMan->PushID(id++);

	ImGuiCameraInfo(lightCamera_.get(), "LightCamera");
	
	imguiMan->EndWindow();
}

ICamera* CameraManager::GetCamera()
{
	if (isDebug_) return debugCamera_.get();

	return mainCamera_.get();
}

ICamera* CameraManager::GetLightCamera()
{
	return lightCamera_.get();
}

ICamera* CameraManager::GetMainCamera()
{
	return mainCamera_.get();
}
