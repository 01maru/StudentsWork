#include "DebugScene.h"
#include "ModelManager.h"
#include "PipelineManager.h"
#include "ImGuiManager.h"

#include "SceneManager.h"
#include "InputManager.h"

#include "CameraManager.h"
#include "LightManager.h"
#include "TextureManager.h"
#include "Easing.h"

void DebugScene::LoadResources()
{
	TextureManager::GetInstance()->LoadTextureGraph("space.png");

#pragma region Model
	ModelManager* models = ModelManager::GetInstance();
	models->LoadModel("");
	models->LoadModel("ground");
	models->LoadModel("objCube", true);
	models->LoadModel("human", true);
	models->LoadModel("spaceShip");

	ship_ = std::make_unique<SpaceShip>();
	ship_->Initialize(models->GetModel("spaceShip"));
#pragma endregion

	//	地面
	ground_ = std::move(Object3D::Create(models->GetModel("ground")));

	skydome_ = std::move(Object3D::Create(models->GetModel("skydome")));
}

void DebugScene::Initialize()
{
	LightManager::GetInstance()->SetFogStart(0.7f);
	LightManager::GetInstance()->SetFogEnd(3.0);

	LoadResources();
	CameraManager::GetInstance()->GetMainCamera()->Initialize(Vector3D(0.0f,-0.5f,1.0f), Vector3D(0.0f, 1.0f, 0.0f), 18.0f);

	ship_->Start();

	sprite_.Initialize(TextureManager::GetInstance()->GetTextureGraph("space.png"));
	sprite_.SetPosition({ 750,400 });

	counter_.Initialize(30, true);
}

void DebugScene::Finalize()
{
}

void DebugScene::MatUpdate()
{
	ground_->MatUpdate();
	
	skydome_->MatUpdate();
	ship_->MatUpdate();
	sprite_.Update();
}

void DebugScene::Update()
{
#pragma region 更新処理

	counter_.Update();

	if (InputManager::GetInstance()->GetTriggerKeyAndButton(DIK_SPACE, InputJoypad::A_Button))
	{
		SceneManager::GetInstance()->SetNextScene("TITLESCENE");
		//ship_->OpenDoor();
	}
	float fade = Easing::EaseIn(0.0f, 1.0f, counter_.GetCountPerMaxCount(), 2);
	sprite_.SetColor({ 1.0f,1.0f,1.0f,fade });
	ship_->Update();
	if (ship_->GetIsEnd()) {
		counter_.StartCount();
		ship_->SetIsEnd(false);
	}
#pragma endregion

	MatUpdate();
}

void DebugScene::ImguiUpdate()
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("DebugScene");

	imguiMan->Text("AnimationIndex : %d", index);

	imguiMan->InputInt("AnimationIdx", index);
	imguiMan->InputInt("AnimationTimer", timer);

	if(imguiMan->SetButton("GameScene")){
		SceneManager::GetInstance()->SetNextScene("GAMESCENE");
	}

	if (imguiMan->SetButton("ShipMove"))	ship_->Start();

	if (imguiMan->SetButton("OpenDoor"))	ship_->OpenDoor();

	imguiMan->EndWindow();
}

void DebugScene::DrawShadow()
{
}

void DebugScene::Draw()
{
	ground_->Draw();
	skydome_->Draw();

	ship_->Draw();

	sprite_.Draw();
}
