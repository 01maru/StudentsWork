#include "DebugScene.h"
#include "ModelManager.h"
#include "PipelineManager.h"
#include "ImGuiManager.h"

#include "SceneManager.h"
#include "InputManager.h"

#include "CameraManager.h"
#include "LightManager.h"
#include "TextureManager.h"
#include "Easing.h"

void DebugScene::LoadResources()
{
	TextureManager::GetInstance()->LoadTextureGraph("space.png");

#pragma region Model
	ModelManager* models = ModelManager::GetInstance();
	models->LoadModel("");
	models->LoadModel("ground");
	//models->LoadModel("objCube", true);
	models->LoadModel("mixhuman", true);
	models->LoadModel("escapePod",true);

	ship_ = std::make_unique<EscapePod>();
	ship_->Initialize(models->GetModel("escapePod"));
#pragma endregion

	//	地面
	ground_ = std::move(Object3D::Create(models->GetModel("ground")));

	skydome_ = std::move(Object3D::Create(models->GetModel("skydome")));
}

void DebugScene::Initialize()
{
	//LightManager::GetInstance()->SetFogStart(0.7f);
	//LightManager::GetInstance()->SetFogEnd(3.0);

	LoadResources();
	CameraManager::GetInstance()->GetMainCamera()->Initialize(Vector3D(0.0f,-0.5f,1.0f), Vector3D(0.0f, 1.0f, 0.0f), 18.0f);
}

void DebugScene::Finalize()
{
}

void DebugScene::MatUpdate()
{
	ground_->MatUpdate();
	
	skydome_->MatUpdate();
	ship_->MatUpdate();
}

void DebugScene::Update()
{
#pragma region 更新処理

	ship_->Update();

#pragma endregion

	MatUpdate();
}

void DebugScene::ImguiUpdate()
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("DebugScene");

	imguiMan->Text("AnimationIndex : %d", index);

	imguiMan->InputInt("AnimationIdx", index);
	ship_->GetAnimation()->SetAnimationIdx(index);
	imguiMan->InputInt("AnimationTimer", timer);
	ship_->GetAnimation()->SetAnimatonTimer(static_cast<float>(timer));

	if(imguiMan->SetButton("TitleScene")){
		SceneManager::GetInstance()->SetNextScene("TITLESCENE");
	}
	if(imguiMan->SetButton("GameScene")){
		SceneManager::GetInstance()->SetNextScene("GAMESCENE");
	}

	if (imguiMan->SetButton("ResetPod"))	ship_->ResetAnimation();

	imguiMan->EndWindow();
}

void DebugScene::DrawShadow()
{
}

void DebugScene::Draw()
{
	ground_->Draw();
	skydome_->Draw();

	ship_->Draw();

	ship_->DrawUI();
}
