#include "DebugScene.h"
#include "ModelManager.h"
#include "PipelineManager.h"

void DebugScene::LoadResources()
{
#pragma region Model
	ModelManager* models = ModelManager::GetInstance();
	models->LoadModel("ground");
	models->LoadModel("objCube", true);
	models->LoadModel("moveCube", true);
#pragma endregion

	//	地面
	ground_.reset(Object3D::Create(models->GetModel("ground")));

	fbx_.reset(Object3D::Create(models->GetModel("objCube")));
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
	fbx_->PlayAnimation();
	fbx_->MatUpdate();
}

void DebugScene::Update()
{
#pragma region 更新処理



#pragma endregion

	MatUpdate();
}

void DebugScene::ImguiUpdate()
{
}

void DebugScene::DrawShadow()
{
}

void DebugScene::Draw()
{
	bool drawShadow = false;
	ground_->Draw(drawShadow);
	fbx_->Draw(drawShadow);
}
