#include "MyGame.h"
#include "SceneManager.h"

#include "LightManager.h"
#include "Object3D.h"

void MNE::MyGame::Initialize()
{
	Framework::Initialize();

	LightManager* light = LightManager::GetInstance();

#pragma region SetLight
	//	ライトの初期化
	light->Initialize();
	int32_t lightIdx = 0;
	Vector3D dir(-0.75f, 0.5f, 0.35f);
	light->SetDirLightActive(lightIdx, true);
	light->SetDirLightShadow(lightIdx, true);
	light->SetDirLightDir(lightIdx, dir);
#pragma endregion
	SceneManager::GetInstance()->Initialize();
}

void MNE::MyGame::Update()
{
	Framework::Update();

	SceneManager::GetInstance()->Update();
	LightManager::GetInstance()->Update();
}

void MNE::MyGame::Draw()
{
	SceneManager::GetInstance()->Draw();
}

void MNE::MyGame::Finalize()
{
	SceneManager::GetInstance()->Finalize();
	Framework::Finalize();
}
