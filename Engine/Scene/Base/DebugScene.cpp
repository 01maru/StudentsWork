#include "DebugScene.h"
#include "ModelManager.h"
#include "PipelineManager.h"
#include "ImGuiManager.h"

#include "SceneManager.h"

void DebugScene::LoadResources()
{
#pragma region Model
	ModelManager* models = ModelManager::GetInstance();
	models->LoadModel("");
	models->LoadModel("ground");
	models->LoadModel("objCube", true);
	models->LoadModel("human", true);
#pragma endregion

	//	地面
	ground_.reset(Object3D::Create(models->GetModel("ground")));

	fbx_.reset(Object3D::Create(models->GetModel("human")));
	//fbx_->SetPosition({ 0.0f,1.0f,0.0f });
	//fbx_->SetScale({ 1.0f,2.0f,1.0f });
}

void DebugScene::Initialize()
{
	LoadResources();
}

void DebugScene::Finalize()
{
}

void DebugScene::MatUpdate()
{
	ground_->MatUpdate();
	
	fbx_->MatUpdate(index);
}

void DebugScene::Update()
{
#pragma region 更新処理



#pragma endregion

	MatUpdate();
}

void DebugScene::ImguiUpdate()
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	fbx_->GetModel()->ImGuiUpdate();

	imguiMan->BeginWindow("DebugScene");

	imguiMan->Text("AnimationIndex : %d", index);

	imguiMan->InputInt("AnimationIdx", index);
	imguiMan->InputInt("AnimationTimer", timer);

	fbx_->SetAnimatonTimer((float)timer);

	if(imguiMan->SetButton("GameScene")){
		SceneManager::GetInstance()->SetNextScene("GAMESCENE");
	}

	imguiMan->EndWindow();
}

void DebugScene::DrawShadow()
{
}

void DebugScene::Draw()
{
	bool drawShadow = false;
	ground_->DrawDissolve(drawShadow);
	fbx_->Draw(drawShadow);
}
