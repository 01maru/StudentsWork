#include "MyGame.h"
#include "SceneManager.h"

#include "LightManager.h"
#include "Object3D.h"

void MyGame::Initialize()
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

void MyGame::Update()
{
	Framework::Update();

	SceneManager::GetInstance()->Update();
	LightManager::GetInstance()->Update();
}

void MyGame::Draw()
{
	SceneManager::GetInstance()->Draw();
}

void MyGame::Finalize()
{
	SceneManager::GetInstance()->Finalize();
	Framework::Finalize();
}
