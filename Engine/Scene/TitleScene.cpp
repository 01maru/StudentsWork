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

using namespace MNE;

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
	//	リスポーン位置設定
	bonfire_ = std::make_unique<Bonfire>();
	bonfire_->Initialize();
	bonfire_->SetModel(models->GetModel("bonfire"));
	bonfire_->Start();
	//	 カメラ位置設定
	std::unique_ptr<ICamera> camera = std::make_unique<TitleCamera>();
	TitleCamera* titleCamera = dynamic_cast<TitleCamera*>(camera.get());
	CameraData cameraData = level_->GetCameraData("Title");
	titleCamera->Initialize(cameraData.eye, cameraData.target, Vector3D(0, 1, 0));
	cameraData = level_->GetCameraData("Option");
	titleCamera->SetCameraPos(cameraData.eye, cameraData.target, TitleCamera::Option);
	cameraData = level_->GetCameraData("ChangeScene");
	titleCamera->SetCameraPos(cameraData.eye, cameraData.target, TitleCamera::SceneChange);
	titleCamera->SetAnimationTime(30);
	CameraManager::GetInstance()->SetMainCamera(camera);

#pragma endregion

#pragma region UI

	uiData_.Initialize();
	uiData_.SetTitleCamera(titleCamera);

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
	bonfire_->MatUpdate();

	ParticleManager::GetInstance()->MatUpdate();
}

void TitleScene::ImguiUpdate()
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("TitleScene");
	TitleCamera* titleCamera = dynamic_cast<TitleCamera*>(CameraManager::GetInstance()->GetMainCamera());

	if (imguiMan->SetButton("MoveMenu")) titleCamera->SetNextMode(TitleCamera::Menu);

	if (imguiMan->SetButton("MoveOption")) titleCamera->SetNextMode(TitleCamera::Option);

	if (imguiMan->SetButton("MoveChangeScene")) titleCamera->SetNextMode(TitleCamera::SceneChange);

	imguiMan->CheckBox("DrawUI", drawUI_);

	uiData_.ImGuiUpdate();

	imguiMan->EndWindow();
}

void TitleScene::Draw()
{
	//	ステージ描画
	level_->Draw(false);
	bonfire_->Draw();

	ParticleManager::GetInstance()->Draw();

	if (!drawUI_) return;
	uiData_.Draw();
}
