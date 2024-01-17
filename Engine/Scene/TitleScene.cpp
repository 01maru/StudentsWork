#include "TitleScene.h"
#include "InputManager.h"
#include "XAudioManager.h"

#include "ImGuiManager.h"
#include "CameraManager.h"
#include "TitleCamera.h"
#include "ModelManager.h"
#include "LightManager.h"
#include "ParticleManager.h"
#include "JSONLoader.h"

using namespace MNE;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void TitleScene::LoadResources()
{
#pragma region Sound
	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	//	タイトル音
	xAudioMan->LoadSoundWave("title.wav");
#pragma endregion

#pragma region Model
	ModelManager* models = ModelManager::GetInstance();
	models->LoadModel("bonfire");

#pragma endregion

#pragma region LevelDatum

	//	ステージ読み込み
	JSONLoader level;
	JSONData levelData = level.LoadJSON("Title");
	//	 カメラ位置設定
	std::unique_ptr<ICamera> camera = std::make_unique<TitleCamera>();
	TitleCamera* titleCamera = dynamic_cast<TitleCamera*>(camera.get());
	CameraData cameraData = levelData.GetCameraData("Title");
	titleCamera->Initialize(cameraData.eye, cameraData.target, Vector3D(0, 1, 0));
	cameraData = levelData.GetCameraData("Option");
	titleCamera->SetCameraPos(cameraData.eye, cameraData.target, TitleCamera::Option);
	cameraData = levelData.GetCameraData("ChangeScene");
	titleCamera->SetCameraPos(cameraData.eye, cameraData.target, TitleCamera::SceneChange);
	titleCamera->SetAnimationTime(30);
	CameraManager::GetInstance()->SetMainCamera(camera);

	//	LightCamera
	CameraManager::GetInstance()->GetLightCamera()->SetEye(Vector3D(78.0f, 50.0f, -30.0f));

	levelData.SetObjects(objs_);

	//	リスポーン位置設定
	bonfire_ = std::make_unique<Bonfire>();
	bonfire_->Initialize();
	bonfire_->SetModel(models->GetModel("bonfire"));
	bonfire_->Start();

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
}

//-----------------------------------------------------------------------------
// [SECTION] Finalize
//-----------------------------------------------------------------------------

void TitleScene::Finalize()
{
	XAudioManager::GetInstance()->StopAllSound();
	XAudioManager::GetInstance()->DeleteAllSound();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

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
	for (auto& obj : objs_)
	{
		obj->MatUpdate();
	}
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

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void TitleScene::DrawUIBeforeBlackScreen()
{
}

void TitleScene::DrawUIAfterBlackScreen()
{
	if (!drawUI_) return;

	uiData_.Draw();
}

void TitleScene::Draw()
{
	//	ステージ描画
	for (auto& obj : objs_)
	{
		obj->Draw();
	}
	bonfire_->Draw();

	ParticleManager::GetInstance()->Draw();
}
