#include "DebugScene.h"
#include "ModelManager.h"
#include "PipelineManager.h"
#include "ImGuiManager.h"

#include "SceneManager.h"
#include "InputManager.h"

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

	skydome_.reset(Object3D::Create(models->GetModel("skydome")));
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
	
	skydome_->MatUpdate(index);
}

void DebugScene::Update()
{
#pragma region 更新処理


	if (InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button))
	{
		SceneManager::GetInstance()->SetNextScene("TITLESCENE");
	}


#pragma endregion

	MatUpdate();
}

void DebugScene::ImguiUpdate()
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("DebugScene");

	imguiMan->Text("AnimationIndex : %d", index);

	imguiMan->InputInt("AnimationIdx", index);
	imguiMan->InputInt("AnimationTimer", timer);

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
	ground_->Draw(drawShadow);
	skydome_->Draw(drawShadow);
}
