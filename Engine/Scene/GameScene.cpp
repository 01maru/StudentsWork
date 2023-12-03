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

#include "GameOverCamera.h"

#include "UISprite.h"

void GameScene::LoadResources()
{
#pragma region Model
	ModelManager* models = ModelManager::GetInstance();
	models->LoadModel("skydome");
	models->LoadModel("ground");
	models->LoadModel("objCube");
	models->LoadModel("chr_sword");
	models->LoadModel("bullet");
	models->LoadModel("eye");
	models->LoadModel("human", true);
	models->LoadModel();
#pragma endregion
	level.LoadJSON("game");
	//	地面
	ground_ = std::move(Object3D::Create(models->GetModel("ground")));
	PlaneCollider* coll_ = new PlaneCollider({ 0.0f,1.0f,0.0f });
	coll_->SetAttribute(CollAttribute::COLLISION_ATTR_LANDSHAPE);
	ground_->SetCollider(coll_);
	//	天球
	skydome_ = std::move(Object3D::Create(models->GetModel("skydome")));
	//collider_ = new MeshCollider;
	//collider_->ConstructTriangles(skydome_->GetModel());
	//collider_->SetAttribute(CollAttribute::COLLISION_ATTR_LANDSHAPE);
	//skydome_->SetCollider(collider_);
	//	player
	player_ = std::make_unique<Player>();
	player_->Initialize(models->GetModel(""));
	//	enemy
	enemy_ = std::make_unique<Boss>();
	enemy_->Initialize(models->GetModel("eye"));
	//	Cube
	cube_ = std::move(Object3D::Create(models->GetModel()));
	cube_->SetPosition({ 3.0f,0.0f,3.0f });

	clear_.Initialize();

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

	enemy_->SetPlayer(player_.get());

	std::unique_ptr<ICamera> camera = std::make_unique<GameCamera>();
	camera->Initialize(Vector3D(0, 0, 1), player_->GetPosition(), 10.0f);
	CameraManager::GetInstance()->SetMainCamera(camera);

	//XAudioManager::GetInstance()->PlaySoundWave("gameBGM.wav", XAudioManager::BGM, true);

	CameraManager::GetInstance()->GetLightCamera()->SetEye(Vector3D(78.0f, 50.0f, -30.0f));
}

void GameScene::Finalize()
{
	ModelManager* models = ModelManager::GetInstance();
	models->DeleteModel("skydome");

	XAudioManager::GetInstance()->StopAllSound();
	XAudioManager::GetInstance()->DeleteAllSound();
}

void GameScene::FirstFrameUpdate()
{
}

void GameScene::MatUpdate()
{
	ParticleManager::GetInstance()->MatUpdate();
	ground_->MatUpdate();
	skydome_->MatUpdate();

	cube_->MatUpdate();
	player_->MatUpdate();
	enemy_->MatUpdate(false);

	level.MatUpdate();
}

void GameScene::Update()
{
#pragma region 更新処理
	//bool select = InputManager::GetInstance()->GetKeyAndButton(DIK_RETURN, InputJoypad::START_Button);

	//if (select) {
	//	InputManager::GetInstance()->GetMouse()->SetLockCursor(false);
	//	CameraManager* cameraMan = CameraManager::GetInstance();
	//	std::unique_ptr<GameOverCamera> camera = std::make_unique<GameOverCamera>();
	//	camera->Initialize(cameraMan->GetMainCamera()->GetEye()
	//		, cameraMan->GetMainCamera()->GetTarget()
	//		, cameraMan->GetMainCamera()->GetUp());
	//	cameraMan->SetMainCamera(std::move(camera));
	//}
	if (enemy_->GetIsHide() == true) {
		clear_.Start();
	}

	pause_.Update();

	if (pause_.GetIsActive() == false)
	{
		ParticleManager::GetInstance()->Update();

	}
		player_->Update();
		enemy_->Update();

	//DebugTextManager::GetInstance()->Print("test", { 0,Window::sWIN_HEIGHT/2.0f }, 5);
#pragma endregion
	MatUpdate();

	clear_.Update();

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

void GameScene::DrawShadow()
{
}

void GameScene::Draw()
{
	////	天球
	//skydome_->Draw();
	////	地面
	//ground_->Draw();
	player_->Draw();
	player_->DrawBullets();
	enemy_->Draw();

	level.Draw(false);

	enemy_->DrawUI();
	player_->DrawUI();

	clear_.Draw();

	pause_.Draw();

	//DebugTextManager::GetInstance()->Draw();
	ParticleManager::GetInstance()->Draw();
}
