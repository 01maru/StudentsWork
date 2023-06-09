﻿#include "GameScene.h"
#include "GameCamera.h"
#include "XAudioManager.h"
#include "TextureManager.h"
#include "ObjModel.h"
#include "FbxModel.h"
#include "PipelineManager.h"
#include "ParticleManager.h"
#include "DebugTextManager.h"
#include "SceneManager.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "MeshCollider.h"

#include "InputManager.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "BoxModel.h"
#include "PauseScreen.h"
#include "ImGuiManager.h"
#include "ModelManager.h"

void GameScene::LoadResources()
{
#pragma region Model
	ModelManager* models = ModelManager::GetInstance();
	models->LoadModel("skydome");
	models->LoadModel("ground");
	models->LoadModel("objCube");
	models->LoadModel("chr_sword");
	models->LoadModel();
#pragma endregion
	//	天球
	skydome_.reset(Object3D::Create(models->GetModel("skydome")));
	//	地面
	ground_.reset(Object3D::Create(models->GetModel("ground")));
	MeshCollider* collider_ = new MeshCollider;
	collider_->ConstructTriangles(ground_->GetModel());
	collider_->SetAttribute(CollAttribute::COLLISION_ATTR_LANDSHAPE);
	ground_->SetCollider(collider_);
	//	player
	player_ = std::make_unique<Player>();
	player_->Initialize(models->GetModel());
	//	enemy
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(models->GetModel());
	//	Cube
	cube_.reset(Object3D::Create(models->GetModel()));
	cube_->SetPosition({ 3.0f,0.0f,3.0f });
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
	pause_ = std::make_unique<PauseScreen>();
	pause_->Initialize();

	Object3D::SetPipeline(PipelineManager::GetInstance()->GetPipeline("Model", GPipeline::ALPHA_BLEND));
	LoadResources();

	level.LoadJSON("gamescene");

	player_->SetPosition(level.GetPlayerSpownPoint().pos);
	//player_->SetRotation(level.GetPlayerSpownPoint().rotation);

	std::unique_ptr<GameCamera> camera = std::make_unique<GameCamera>();
	camera->Initialize(Vector3D(0, 0, 1), player_->GetPosition(), 10.0f);
	CameraManager::GetInstance()->SetMainCamera(std::move(camera));

	XAudioManager::GetInstance()->PlaySoundWave("gameBGM.wav", XAudioManager::BGM, true);

	CameraManager::GetInstance()->GetLightCamera()->SetEye(Vector3D(78.0f, 50.0f, -30.0f));
}

void GameScene::Finalize()
{
	ModelManager* models = ModelManager::GetInstance();
	models->DeleteModel("skydome");

	XAudioManager::GetInstance()->StopAllSound();
	XAudioManager::GetInstance()->DeleteAllSound();
}

void GameScene::MatUpdate()
{
	ParticleManager::GetInstance()->MatUpdate();
	ground_->MatUpdate();
	skydome_->MatUpdate();

	cube_->MatUpdate();
	player_->MatUpdate();
	enemy_->MatUpdate();
	//player_->PlayAnimation();

	level.MatUpdate();
}

void GameScene::Update()
{
#pragma region 更新処理
	if (InputManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_B)) {
		SceneManager::GetInstance()->SetNextScene("TITLESCENE");
	}

	sprite_->Update();
	ParticleManager::GetInstance()->Update();

	player_->Update();
	enemy_->Update();

	//DebugTextManager::GetInstance()->Print("test", { 0,Window::sWIN_HEIGHT/2.0f }, 5);
#pragma endregion
	MatUpdate();

	player_->CollisionUpdate();

	CollisionManager::GetInstance()->CheckAllCollisions();
}

void GameScene::ImguiUpdate()
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("GameScene", true);


	imguiMan->Text("mord : %d",pause_->GetMord());

	if (imguiMan->SetButton("Continue"))	pause_->SetMord(PauseScreen::Continue);
	if (imguiMan->SetButton("Option"))		pause_->SetMord(PauseScreen::Option);
	if (imguiMan->SetButton("BackTitle"))	pause_->SetMord(PauseScreen::BackTitle);

	imguiMan->Text("Option : %s", pause_->GetOptionIsActive() ? "True" : "False");
	imguiMan->Text("OptionMord : %d",pause_->GetOptionMord());

	imguiMan->EndWindow();
}

void GameScene::DrawShadow()
{
	//cube_->DrawShadow();
}

void GameScene::Draw()
{
	//	天球
	skydome_->DrawShadowReciever();
	////	地面
	ground_->DrawShadowReciever();
	//cube_->DrawShadowReciever();
	player_->DrawShadowReciever();
	enemy_->Draw();

	//level.Draw();

	//sprite_->Draw();

	//DebugTextManager::GetInstance()->Draw();
	ParticleManager::GetInstance()->Draw();
}
