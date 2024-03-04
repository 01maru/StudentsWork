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
	stateTable_ = { &GameScene::PauseScene,&GameScene::StartScene,&GameScene::PlayScene,&GameScene::EndScene };
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

	enemy_->SetPlayer(player_.get());

	camera_ = dynamic_cast<GameCamera*>(CameraManager::GetInstance()->GetMainCamera());
	camera_->SetEnemyPos(enemy_->GetPositionPtr());
	camera_->SetPlayerPos(player_->GetPositionPtr());
	player_->SetGameCamera(camera_);
	pause_.SetGameCamera(camera_);

	std::unique_ptr<Cylinder> stageColl = std::make_unique<Cylinder>();
	stageColl->radius_ = 60.0f;
	CollisionManager::GetInstance()->AddStageCollider(stageColl);

	enemy_->SetGameScene(this);
	player_->SetGameScene(this);
	pod_.SetGameScene(this);
	pause_.SetGameScene(this);
	nowState_ = StartState;

	playerBullets_.Initialize();
	enemyBullets_.Initialize();
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

void GameScene::PauseScene()
{
	//	ポーズの更新
	pause_.IsActiveUpdate();

	pause_.Update();
}

void GameScene::StartScene()
{
	//	ポーズの更新
	pause_.IsActiveUpdate();

	pod_.Update();
}

void GameScene::PlayScene()
{
	//	ポーズの更新
	pause_.IsActiveUpdate();

	PlayGameUpdate();
}

void GameScene::EndScene()
{
	PlayGameUpdate();

	clear_->Update();
	gameOver_->Update();
}

void GameScene::PlayGameUpdate()
{
	pod_.Update();
	player_->Update();
	playerBullets_.Update(player_->GetBullets());
	enemy_->Update();
	enemyBullets_.Update(enemy_->GetBullets());

	CollisionUpdate();
}

void GameScene::MatUpdate()
{
	pause_.AnimationUpdate();

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

void GameScene::CollisionUpdate()
{
	player_->CollisionUpdate();
	enemy_->CollisionUpdate();

	playerBullets_.CollisionUpdate();
	enemyBullets_.CollisionUpdate();
}

void GameScene::Update()
{
#pragma region 更新処理

	camera_->SetIsActive(nowState_ != StartState && pause_.GetIsActive() == FALSE);

	//	現在のステートの更新
	(this->*stateTable_[nowState_])();

	//	パーティクルマネージャの更新
	ParticleManager::GetInstance()->Update();

	//	黒帯の更新
	letterBox_.Update();

	MatUpdate();

#pragma endregion
}

//-----------------------------------------------------------------------------
// [SECTION] ImGuiUpdate
//-----------------------------------------------------------------------------

void GameScene::ImguiUpdate()
{
	ImGuiManager* imGuiMan = ImGuiManager::GetInstance();

	player_->ImGuiUpdate();

	imGuiMan->BeginWindow("GameScene", true);

	imGuiMan->Text("NowState : %d", nowState_);

	if (imGuiMan->SetButton("BossActive")) {
		enemy_->SetIsActive(true);
	}

	if (imGuiMan->SetButton("ResetGameOverAnime")) {
		gameOver_->Reset();
	}

	if (imGuiMan->SetButton("ResetPod"))	pod_.ResetAnimation();

	enemy_->ImGuiUpdate();

	pause_.ImGuiUpdate();

	imGuiMan->EndWindow();
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void GameScene::DrawUIBeforeBlackScreen()
{
	if (drawPlayer_ == TRUE) {
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
	//	地形の描画
	for (auto& obj : objs_)
	{
		obj->Draw();
	}

	if (drawPlayer_ == TRUE) {

		player_->Draw();
		playerBullets_.Draw();
	}
	enemy_->Draw();
	enemyBullets_.Draw();
	
	//	脱出ポッド
	pod_.Draw();

	ParticleManager::GetInstance()->Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

int32_t GameScene::GetNowState()
{
	return nowState_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void GameScene::SetNextState(int32_t nextState)
{
	nowState_ = nextState;
}

void GameScene::SetDrawPlayer(bool drawPlayer)
{
	drawPlayer_ = drawPlayer;
}

void GameScene::ActiveGameOver()
{
	if (gameOver_->GetIsActive() == TRUE) return;

	gameOver_->Start();
	gameOver_->SetCameraPosData(player_->GetPosition());
}

void GameScene::ActiveClearState()
{
	if (clear_->GetIsActive() == TRUE) return;

	clear_->Start();
}
