#include "Framework.h"
#include "ParticleManager.h"
#include "DebugTextManager.h"
#include "TextureManager.h"
#include "PipelineManager.h"
#include "DirectX.h"
#include "Window.h"

#include "InputManager.h"
#include "XAudioManager.h"
#include "FPSController.h"
#include "CameraManager.h"
#include "SceneManager.h"

void MNE::Framework::Run()
{
	//	初期化
	Initialize();

	//	ゲームループ
	while (gameroopFlag_)
	{
		//	更新
		Update();

		//	描画
		Draw();
	}

	Finalize();
}

void MNE::Framework::Initialize()
{
	//	winApi初期化
	Window::GetInstance()->Initialize();

	MyDirectX::GetInstance()->Initialize();

	PipelineManager::GetInstance()->Initialize();

	TextureManager::GetInstance()->Initialize();

	InputManager::GetInstance()->Initialize();

	ParticleManager::GetInstance()->Initialize();

	DebugTextManager::GetInstance()->Initialize();

	FPSController::GetInstance()->Initialize();

	XAudioManager::GetInstance()->Initialize();

	CameraManager::GetInstance()->Initialize();
}

void MNE::Framework::Update()
{
	gameroopFlag_ = !(Window::GetInstance()->MsgUpdate() || !SceneManager::GetInstance()->GetGameLoop());

	InputManager::GetInstance()->Update();

	//	60fps固定
	FPSController::GetInstance()->Update();
}

void MNE::Framework::Finalize()
{
	XAudioManager::GetInstance()->Finalize();
	Window::GetInstance()->Finalize();
}
