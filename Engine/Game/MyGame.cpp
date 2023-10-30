#include "MyGame.h"
#include "SceneManager.h"

#include "LightManager.h"
#include "Object3D.h"
#include "GameOverUI.h"

void MyGame::Initialize()
{
	Framework::Initialize();

	LightManager* light = LightManager::GetInstance();

#pragma region SetLight
	//	ライトの初期化
	light->Initialize();
	light->SetDirLightActive(0, true);
	light->SetDirLightShadow(0, true);
	light->SetDirLightDir(0, Vector3D(-0.75f, 0.5f, 0.35f));
#pragma endregion

	GameOverUI::GetInstance()->Initialize();

	SceneManager::GetInstance()->Initialize();
}

void MyGame::Update()
{
	Framework::Update();

	GameOverUI::GetInstance()->Update();

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
