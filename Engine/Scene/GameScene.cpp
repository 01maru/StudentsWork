﻿#include "GameScene.h"
#include "MyDebugCamera.h"
#include "GameCamera.h"
#include "XAudioManager.h"
#include "NormalCamera.h"
#include "TextureManager.h"
#include "ObjModel.h"
#include "PipelineManager.h"
#include "ParticleManager.h"
#include "DebugTextManager.h"
#include "SceneManager.h"

#include "InputManager.h"

void GameScene::LoadResources()
{
#pragma region Model
	modelSkydome_ = std::make_unique<ObjModel>("skydome");
	modelGround_ = std::make_unique<ObjModel>("ground");
	modelCube_ = std::make_unique<ObjModel>("objCube");
#pragma endregion
	//	天球
	skydome_.reset(Object3D::Create(modelSkydome_.get()));
	//	地面
	ground_.reset(Object3D::Create(modelGround_.get()));
	//	Cube
	cube_.reset(Object3D::Create(modelCube_.get()));
	cube_->SetScale({ 5.0f,5.0f,5.0f });
#pragma region Texture
	reimuG = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/reimu.png");
	grassG = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/grass.png");
#pragma endregion

#pragma region Sprite
	sprite_ = std::make_unique<Sprite>();
	sprite_->SetSize(Vector2D(200.0f, 200.0f));
	sprite_->Initialize(reimuG);
#pragma endregion

#pragma region Sound
	XAudioManager::GetInstance()->LoadSoundWave("gameBGM.wav");
#pragma endregion
}

void GameScene::Initialize()
{
	camera = new MyDebugCamera();
	camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	//camera = Light::GetInstance()->GetDirLightCamera(0);

	Object3D::SetPipeline(PipelineManager::GetInstance()->GetPipeline("Model", GPipeline::ALPHA_BLEND));
	Object3D::SetCamera(camera);
	LoadResources();

	XAudioManager::GetInstance()->PlaySoundWave("gameBGM.wav", XAudioManager::BGM, true);
}

void GameScene::Finalize()
{
	XAudioManager::GetInstance()->StopAllSound();
	XAudioManager::GetInstance()->DeleteAllSound();
}

void GameScene::MatUpdate()
{
	ParticleManager::GetInstance()->MatUpdate();
	ground_->MatUpdate();
	skydome_->MatUpdate();

	cube_->MatUpdate();
}

void GameScene::Update()
{
#pragma region 更新処理
	if (InputManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_B)) {
		SceneManager::GetInstance()->SetNextScene("TITLESCENE");
	}

	camera->Update();

	sprite_->Update();
	ParticleManager::GetInstance()->Update();

	//DebugTextManager::GetInstance()->Print("test", { 0,Window::sWIN_HEIGHT/2.0f }, 5);
#pragma endregion
	MatUpdate();

	Light::GetInstance()->SetDirLightColor(0, Vector3D(1.0f, 1.0f, 1.0f));
}

void GameScene::ImguiUpdate()
{
}

void GameScene::DrawShadow()
{
}

void GameScene::Draw()
{
	//	天球
	skydome_->Draw();
	//	地面
	//ground->Draw();
	cube_->Draw();

	//sprite_->Draw();

	//DebugTextManager::GetInstance()->Draw();
	ParticleManager::GetInstance()->Draw();
}
