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
	light->SetDirLightActive(0, true);
	light->SetDirLightShadow(0, true);
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
