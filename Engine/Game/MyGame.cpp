#include "MyGame.h"
#include "SceneManager.h"

#include "Light.h"
#include "Object3D.h"
#include "CameraManager.h"

void MyGame::Initialize()
{
	Framework::Initialize();

	Light* light = Light::GetInstance();

#pragma region SetLight
	light->SetDirLightActive(0, true);
	light->SetDirLightShadow(0, true);
	//	ライトの初期化
	light->Initialize();
#pragma endregion

	SceneManager::GetInstance()->Initialize();
}

void MyGame::Update()
{
	Framework::Update();

	CameraManager::GetInstance()->Update();

	SceneManager::GetInstance()->Update();
	Light::GetInstance()->Update();
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
