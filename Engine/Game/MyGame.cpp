#include "MyGame.h"
#include "SceneManager.h"
#include "LightManager.h"

using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------
// [SECTION] Finalize
//-----------------------------------------------------------------------------

void MNE::MyGame::Finalize()
{
	SceneManager::GetInstance()->Finalize();
	Framework::Finalize();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void MNE::MyGame::Update()
{
	Framework::Update();

	SceneManager::GetInstance()->Update();

	LightManager::GetInstance()->Update();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void MNE::MyGame::Draw()
{
	SceneManager::GetInstance()->Draw();
}
