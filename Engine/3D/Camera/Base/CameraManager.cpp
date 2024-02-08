#include "CameraManager.h"
#include "ImGuiController.h"
#include "ImGuiManager.h"

#include "MyDebugCamera.h"
#include "ModelManager.h"

using namespace MyMath;

MNE::CameraManager* MNE::CameraManager::GetInstance()
{
    static CameraManager instance;
    return &instance;
}

void MNE::CameraManager::Initialize()
{
#ifdef NDEBUG
	isDebug_ = false;
#endif // NDEBUG

#ifdef _DEBUG
	debugCamera_ = std::make_unique<MyDebugCamera>();
	debugCamera_->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	targetObj_ = std::move(Object3D::Create(ModelManager::GetInstance()->GetModel()));
	float scale = 0.5f;
	targetObj_->SetScale(Vector3D(scale, scale, scale));
	float alpha = 0.2f;
	targetObj_->SetAlphaColor(alpha);
#endif // _DEBUG
}

void MNE::CameraManager::TargetUpdate()
{
	if (!drawTarget_) return;

	ICamera* main = mainCamera_.get();
	if (isDebug_) main = debugCamera_.get();
	if (lightView_) main = lightCamera_.get();

	targetObj_->SetPosition(main->GetTarget());
	targetObj_->MatUpdate();
}

void MNE::CameraManager::Update()
{
	if (isDebug_)						debugCamera_->Update();
	if (mainCamera_ != nullptr)			mainCamera_->Update();
	if (lightCamera_ != nullptr)		lightCamera_->Update();
	if (orthoProjCamera_ != nullptr)	orthoProjCamera_->Update();

	TargetUpdate();
}

void MNE::CameraManager::SetDebugCameraPosToMain()
{
	//	mainCameraがセットされていなかったら
	if (mainCamera_ == nullptr) return;

	debugCamera_->Initialize(mainCamera_->GetEye(), mainCamera_->GetTarget(), mainCamera_->GetUp());
}

void MNE::CameraManager::ImGuiCameraInfo(ICamera* camera, const std::string& name)
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

		camera->ImGuiUpdate();
	}

	imguiMan->PopID();
}

void MNE::CameraManager::ImGuiUpdate()
{
    if (!ImGuiController::GetInstance()->GetActiveCameraManager()) return;

	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("CameraManager", true);

	imguiMan->CheckBox("IsDebug", isDebug_);
	imguiMan->CheckBox("LightCameraView", lightView_);

	imguiMan->Spacing();
	imguiMan->Separator();
	imguiMan->Spacing();

	imguiMan->CheckBox("DrawTarget", drawTarget_);


	if (imguiMan->SetButton("SetDebugPos = MainCamera")) SetDebugCameraPosToMain();
	int32_t id = 0;

	imguiMan->PushID(id++);

	ImGuiCameraInfo(mainCamera_.get(), "MainCamera");

	imguiMan->PushID(id++);

	ImGuiCameraInfo(debugCamera_.get(), "DebugCamera");

	imguiMan->PushID(id++);

	ImGuiCameraInfo(lightCamera_.get(), "LightCamera");
	
	imguiMan->EndWindow();
}

void MNE::CameraManager::DrawTarget()
{
	if (!drawTarget_) return;

	targetObj_->Draw();
}

MNE::ICamera* MNE::CameraManager::GetCamera()
{
	if (lightView_) return lightCamera_.get();
	if (isDebug_) return debugCamera_.get();

	return mainCamera_.get();
}

MNE::ICamera* MNE::CameraManager::GetLightCamera()
{
	return lightCamera_.get();
}

MNE::ICamera* MNE::CameraManager::GetMainCamera()
{
	return mainCamera_.get();
}

MNE::ICamera* MNE::CameraManager::GetDebugCamera()
{
	return debugCamera_.get();
}

MNE::ICamera* MNE::CameraManager::GetOrthoProjCamera()
{
	return orthoProjCamera_.get();
}
