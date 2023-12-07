#include "TitleScene.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "XAudioManager.h"
#include "Window.h"

#include "ImGuiManager.h"
#include "CameraManager.h"
#include "TitleCamera.h"
#include "ModelManager.h"
#include "LightManager.h"
#include "ParticleManager.h"

#include "FireParticleEmitter.h"
#include "SmokeParticleEmitter.h"

void TitleScene::LoadResources()
{
#pragma region Sound
	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	//	タイトル音
	xAudioMan->LoadSoundWave("title.wav");
#pragma endregion

#pragma region Model
	ModelManager* models = ModelManager::GetInstance();
	models->LoadModel("grass");
	models->LoadModel("midTree");
	models->LoadModel("lowTree");
	models->LoadModel("ground");
	models->LoadModel("skydome");
	models->LoadModel("bonfire");

	//	ステージ読み込み
	level_ = std::make_unique<JSONLoader>();
	level_->LoadJSON("Title");
	//	天球
	skydome_ = std::move(Object3D::Create(models->GetModel("skydome")));
	//	地面
	ground_ = std::move(Object3D::Create(models->GetModel("ground")));
	bonfire_ = std::move(Object3D::Create(models->GetModel("bonfire")));

#pragma endregion
	
	SmokeParticleEmitter smokeEmitter;
	ParticleManager::GetInstance()->AddEmitter(smokeEmitter.GetEmitter());
	FireParticleEmitter emitter;
	ParticleManager::GetInstance()->AddEmitter(emitter.GetEmitter());

#pragma region UI

	uiData_.Initialize();

#pragma endregion
}

void TitleScene::Initialize()
{
	//	Fog
	LightManager::GetInstance()->SetFogActive(true);
	LightManager::GetInstance()->SetFogStart(2.2f);
	LightManager::GetInstance()->SetFogEnd(6.0f);
	LightManager::GetInstance()->SetFogFar(30.0f);

	//	カーソル固定解除
	InputManager::GetInstance()->GetMouse()->SetLockCursor(false);

	LoadResources();

	//	Camera
	std::unique_ptr<ICamera> camera = std::make_unique<TitleCamera>();
	camera->Initialize(Vector3D(0.5f, 12.0f, -22.0f), Vector3D(1.0f,3.0f,0.5f), Vector3D(0, 1, 0));
	CameraManager::GetInstance()->SetMainCamera(camera);
	//	LightCamera
	CameraManager::GetInstance()->GetLightCamera()->SetEye(Vector3D(78.0f, 50.0f, -30.0f));
}

void TitleScene::Finalize()
{
	XAudioManager::GetInstance()->StopAllSound();
	XAudioManager::GetInstance()->DeleteAllSound();
}

void TitleScene::FirstFrameUpdate()
{
	//XAudioManager::GetInstance()->PlaySoundWave("title.wav", XAudioManager::BGM, true);

	uiData_.Start();
}

void TitleScene::Update()
{
	uiData_.Update();

	ParticleManager::GetInstance()->Update();
	
	MatUpdate();
}

void TitleScene::MatUpdate()
{
	//	モデル
	level_->MatUpdate();
	ground_->MatUpdate();
	skydome_->MatUpdate();
	bonfire_->MatUpdate();

	ParticleManager::GetInstance()->MatUpdate();
}

void TitleScene::ImguiUpdate()
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("TitleScene");

	imguiMan->CheckBox("DrawUI", drawUI_);

	uiData_.ImGuiUpdate();

	imguiMan->EndWindow();
}

void TitleScene::Draw()
{
	//	ステージ描画
	level_->Draw(false);
	ground_->Draw();
	bonfire_->Draw();
	skydome_->Draw();

	ParticleManager::GetInstance()->Draw();

	if (!drawUI_) return;
	uiData_.Draw();
}
