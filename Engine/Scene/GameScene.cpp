#include "GameScene.h"
#include "GameCamera.h"
#include "XAudioManager.h"
#include "ParticleManager.h"
#include "SceneManager.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "MeshCollider.h"
#include "PlaneCollider.h"

#include "CameraManager.h"
#include "ImGuiManager.h"
#include "ModelManager.h"

#include "UISprite.h"

#include "ClearUI.h"
#include "GameOverUI.h"
#include "JSONLoader.h"

using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void GameScene::LoadResources()
{
#pragma region Model
	ModelManager* models = ModelManager::GetInstance();
	models->LoadModel("Boss", true);
	models->LoadModel("player", true);
	models->LoadModel("escapePod", true);
#pragma endregion

	//	地面
	ground_ = std::move(MNE::Object3D::Create(models->GetModel("ground")));
	Vector3D upDir(0.0f, 1.0f, 0.0f);
	PlaneCollider* coll_ = new PlaneCollider(upDir);
	coll_->SetAttribute(CollAttribute::COLLISION_ATTR_LANDSHAPE);
	ground_->SetCollider(coll_);
	//	天球
	skydome_ = std::move(MNE::Object3D::Create(models->GetModel("skydome")));
	//	player
	player_ = std::make_unique<Player>();
	player_->Initialize(models->GetModel("player"));
	playerBullets_.LoadResources();
	//	enemy
	enemy_ = std::make_unique<Boss>();
	enemy_->Initialize(models->GetModel("Boss"));

#pragma region LevelData

	JSONLoader level;
	JSONData levelData = level.LoadJSON("game");

	levelData.SetObjects(objs_);

#pragma endregion

#pragma region SetUIInfo

	//	クリア
	clear_ = std::make_unique<ClearUI>();
	clear_->LoadResources();
	//	ゲームオーバー
	gameOver_ = std::make_unique<GameOverUI>();
	gameOver_->LoadResources();

	UIData ui;
	ui.LoadData("GameUI");
	//	Player
	player_->SetUIInfo(ui);

	//	Enemy
	//	HP
	UIObject* gameUIObj = ui.GetUIObject("Enemy");
	UISprite* gameUISprite = gameUIObj->GetComponent<UISprite>();
	enemy_->SetHPBarSprite(gameUISprite->GetSprites()["bossHP"]);

	//	Pod
	//	InputExplain
	gameUIObj = ui.GetUIObject("exitText");
	gameUISprite = gameUIObj->GetComponent<UISprite>();
	pod_.SetInputUISprite(gameUISprite->GetSprites()["exitText"], gameUISprite->GetSprites()["inputButton"]);

	//	ムービー用黒帯
	letterBox_.LoadData("LetterBox");

#pragma endregion

	//	脱出ポッド
	pod_.LoadResources();
	pod_.SetLetterBox(&letterBox_);
	pod_.Initialize({ 0.0f,-0.3f,-50.0f });
	pod_.SetModel(models->GetModel("escapePod"));

#pragma region Sound
	XAudioManager::GetInstance()->LoadSoundWave("gameBGM.wav");
#pragma endregion
}

void GameScene::Initialize()
{
	pause_.Initialize();

	//	ゲームカメラ
	std::unique_ptr<ICamera> camera = std::make_unique<GameCamera>();
	camera->Initialize(Vector3D(0.0f, 6.5f, -70.0f), Vector3D(0.0f, 1.5f, -50.0f), Vector3D(0, 1, 0));
	CameraManager::GetInstance()->SetMainCamera(camera);

	LoadResources();

	gameOver_->Initialize();
	//Vector3D pos = level.GetPlayerSpownPoint().pos;
	//pos.y = 1.0f;
	//player_->SetPosition(pos);
	//player_->SetRotation(level.GetPlayerSpownPoint().rotation);
	player_->SetPosition({ 0.0f,0.0f,-52.0f });
	player_->SetGameOverState(gameOver_.get());

	enemy_->SetPlayer(player_.get());
	enemy_->SetClearState(clear_.get());

	camera_ = dynamic_cast<GameCamera*>(CameraManager::GetInstance()->GetMainCamera());
	camera_->SetEnemyPos(enemy_->GetPositionPtr());
	camera_->SetPlayerPos(player_->GetPositionPtr());
	pause_.SetGameCamera(camera_);

	std::unique_ptr<Cylinder> stageColl = std::make_unique<Cylinder>();
	stageColl->radius_ = 60.0f;
	CollisionManager::GetInstance()->AddStageCollider(stageColl);
}

//-----------------------------------------------------------------------------
// [SECTION] Finalize
//-----------------------------------------------------------------------------

void GameScene::Finalize()
{
	ModelManager* models = ModelManager::GetInstance();
	models->DeleteModel("skydome");

	XAudioManager::GetInstance()->StopAllSound();
	XAudioManager::GetInstance()->DeleteAllSound();

	SceneManager::GetInstance()->ChangeScreenAlpha(0.0f);
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void GameScene::FirstFrameUpdate()
{
	//XAudioManager::GetInstance()->PlaySoundWave("gameBGM.wav", XAudioManager::BGM, true);
}

void GameScene::MatUpdate()
{
	ParticleManager::GetInstance()->MatUpdate();
	ground_->MatUpdate();
	skydome_->MatUpdate();

	player_->MatUpdate();
	enemy_->MatUpdate();

	pod_.MatUpdate();
	for (auto& obj : objs_)
	{
		obj->MatUpdate();
	}
}

void GameScene::InGameUpdate()
{
	//	ポーズ中だったら処理しない
	if (pause_.GetIsActive() == TRUE)	return;
	//	ポーズの切り替わりタイミングだったら処理しない
	if (pauseActiveTrigger_ == TRUE)	return;
	
	ParticleManager::GetInstance()->Update();

	pod_.Update();

	if (pod_.GetOpenDoor() == TRUE)
	{
		player_->Update();
		playerBullets_.Update(player_->GetBullets());
		enemy_->Update();
	}

	UIUpdate();
}

void GameScene::UIUpdate()
{
	letterBox_.Update();
	clear_->Update();
	gameOver_->Update();
}

void GameScene::CollisionUpdate()
{
	//	ポーズ中だったら処理しない
	if (pause_.GetIsActive() == TRUE)	return;
	//	ポーズの切り替わりタイミングだったら処理しない
	if (pauseActiveTrigger_ == TRUE)	return;

	if (pod_.GetOpenDoor() == TRUE)
	{
		player_->CollisionUpdate();
	}

	CollisionManager::GetInstance()->CheckAllCollisions();
}

void GameScene::Update()
{
#pragma region 更新処理
	//	ポーズの更新
	pauseActiveTrigger_ = pause_.Update();

	camera_->SetIsActive(pod_.GetOpenDoor() && pause_.GetIsActive() == FALSE);

	InGameUpdate();

	MatUpdate();

	CollisionUpdate();
#pragma endregion
}

//-----------------------------------------------------------------------------
// [SECTION] ImGuiUpdate
//-----------------------------------------------------------------------------

void GameScene::ImguiUpdate()
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	player_->ImGuiUpdate();

	imguiMan->BeginWindow("GameScene", true);

	if (imguiMan->SetButton("BossActive")) {
		enemy_->SetIsActive(true);
	}

	if (imguiMan->SetButton("ResetGameOverAnime")) {
		gameOver_->Reset();
	}

	if (imguiMan->SetButton("ResetPod"))	pod_.ResetAnimation();

	enemy_->ImGuiUpdate();

	pause_.ImGuiUpdate();

	imguiMan->EndWindow();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void GameScene::DrawUIBeforeBlackScreen()
{
	if (pod_.GetDrawPlayer() == TRUE) {
		enemy_->DrawUI();
		player_->DrawUI();
	}

	clear_->Draw();

	letterBox_.Draw();

	pod_.DrawUI();
}

void GameScene::DrawUIAfterBlackScreen()
{
	gameOver_->Draw();
	pause_.Draw();
}

void GameScene::Draw()
{
	if (pod_.GetDrawPlayer() == TRUE) {

		player_->Draw();
		playerBullets_.Draw();
	}
	enemy_->Draw();
	enemy_->DrawBullets();
	
	//	脱出ポッド
	pod_.Draw();
	//	地形の描画
	for (auto& obj : objs_)
	{
		obj->Draw();
	}

	ParticleManager::GetInstance()->Draw();
}
