#include "GameScene.h"
#include "GameCamera.h"
#include "XAudioManager.h"
#include "ParticleManager.h"
#include "DebugTextManager.h"
#include "SceneManager.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "MeshCollider.h"
#include "PlaneCollider.h"

#include "InputManager.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "ImGuiManager.h"
#include "ModelManager.h"

#include "UISprite.h"

#include "ClearUI.h"
#include "GameOverUI.h"

void GameScene::LoadResources()
{
#pragma region Model
	ModelManager* models = ModelManager::GetInstance();
	models->LoadModel("skydome");
	models->LoadModel("ground");
	models->LoadModel("bullet");
	models->LoadModel("eye");
	models->LoadModel("human", true);
	models->LoadModel();
#pragma endregion
	level.LoadJSON("game");
	//	地面
	ground_ = std::move(Object3D::Create(models->GetModel("ground")));
	Vector3D upDir(0.0f, 1.0f, 0.0f);
	PlaneCollider* coll_ = new PlaneCollider(upDir);
	coll_->SetAttribute(CollAttribute::COLLISION_ATTR_LANDSHAPE);
	ground_->SetCollider(coll_);
	//	天球
	skydome_ = std::move(Object3D::Create(models->GetModel("skydome")));
	//	player
	player_ = std::make_unique<Player>();
	player_->Initialize(models->GetModel(""));
	//	enemy
	enemy_ = std::make_unique<Boss>();
	enemy_->Initialize(models->GetModel("eye"));

	clear_ = std::make_unique<ClearUI>();
	clear_->Initialize();
	gameOver_ = std::make_unique<GameOverUI>();
	gameOver_->Initialize();

	UIData ui;
	ui.LoadData("GameUI");
	UIObject* gameUIObj = ui.GetUIObject("HP");
	UISprite* gameUISprite = gameUIObj->GetComponent<UISprite>();
	player_->SetHPBarSprite(gameUISprite->GetSprites()["hp"]);

	gameUIObj = ui.GetUIObject("crossHair");
	gameUISprite = gameUIObj->GetComponent<UISprite>();
	player_->SetCrossHairSprite(gameUISprite->GetSprites()["crossHair"]);

	gameUIObj = ui.GetUIObject("SlowCool");
	gameUISprite = gameUIObj->GetComponent<UISprite>();
	player_->SetSlowAtCoolSprite(gameUISprite->GetSprites()["SlowAt"], gameUISprite->GetSprites()["Text"]);
	
	gameUIObj = ui.GetUIObject("DashCool");
	gameUISprite = gameUIObj->GetComponent<UISprite>();
	player_->SetAvoidCoolSprite(gameUISprite->GetSprites()["Dash"], gameUISprite->GetSprites()["Text"]);

	gameUIObj = ui.GetUIObject("Enemy");
	gameUISprite = gameUIObj->GetComponent<UISprite>();
	enemy_->SetHPBarSprite(gameUISprite->GetSprites()["bossHP"]);

#pragma region Sound
	XAudioManager::GetInstance()->LoadSoundWave("gameBGM.wav");
#pragma endregion
}

void GameScene::Initialize()
{
	pause_.Initialize();

	LoadResources();

	Vector3D pos = level.GetPlayerSpownPoint().pos;
	pos.y = 1.0f;
	player_->SetPosition(pos);
	//player_->SetRotation(level.GetPlayerSpownPoint().rotation);
	player_->SetGameOverState(gameOver_.get());

	enemy_->SetPlayer(player_.get());
	enemy_->SetClearState(clear_.get());

	std::unique_ptr<ICamera> camera = std::make_unique<GameCamera>();
	camera->Initialize(Vector3D(0, 0, 1), player_->GetPosition(), 10.0f);
	CameraManager::GetInstance()->SetMainCamera(camera);

	pause_.SetGameCamera(dynamic_cast<GameCamera*>(CameraManager::GetInstance()->GetMainCamera()));

	std::unique_ptr<Cylinder> stageColl = std::make_unique<Cylinder>();
	stageColl->radius_ = 60.0f;
	CollisionManager::GetInstance()->AddStageCollider(stageColl);

	//XAudioManager::GetInstance()->PlaySoundWave("gameBGM.wav", XAudioManager::BGM, true);
}

void GameScene::Finalize()
{
	ModelManager* models = ModelManager::GetInstance();
	models->DeleteModel("skydome");

	XAudioManager::GetInstance()->StopAllSound();
	XAudioManager::GetInstance()->DeleteAllSound();

	SceneManager::GetInstance()->ChangeScreenAlpha(0.0f);
}

void GameScene::FirstFrameUpdate()
{
}

void GameScene::MatUpdate()
{
	ParticleManager::GetInstance()->MatUpdate();
	ground_->MatUpdate();
	skydome_->MatUpdate();

	player_->MatUpdate();
	enemy_->MatUpdate();

	level.MatUpdate();
}

void GameScene::Update()
{
#pragma region 更新処理
	pause_.Update();

	if (pause_.GetIsActive() == false)
	{
		ParticleManager::GetInstance()->Update();

		player_->Update();
		enemy_->Update();
	}
#pragma endregion
	MatUpdate();

	clear_->Update();
	gameOver_->Update();

	player_->CollisionUpdate();

	CollisionManager::GetInstance()->CheckAllCollisions();
}

void GameScene::ImguiUpdate()
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	player_->ImGuiUpdate();

	imguiMan->BeginWindow("GameScene", true);

	enemy_->ImGuiUpdate();

	pause_.ImGuiUpdate();

	imguiMan->EndWindow();
}

void GameScene::DrawUIBeforeBlackScreen()
{
	enemy_->DrawUI();
	player_->DrawUI();

	clear_->Draw();
}

void GameScene::DrawUIAfterBlackScreen()
{
	gameOver_->Draw();
	pause_.Draw();
}

void GameScene::Draw()
{
	player_->Draw();
	player_->DrawBullets();
	enemy_->Draw();
	enemy_->DrawBullets();

	level.Draw(false);

	ParticleManager::GetInstance()->Draw();
}
