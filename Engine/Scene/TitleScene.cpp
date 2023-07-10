#include "TitleScene.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "NormalCamera.h"
#include "TextureManager.h"
#include "XAudioManager.h"

#include "PauseScreen.h"

#include "ICamera.h"

void TitleScene::LoadResources()
{
#pragma region Sound
	XAudioManager* xAudioMan = XAudioManager::GetInstance();
	xAudioMan->LoadSoundWave("cursorMove.wav");
	xAudioMan->LoadSoundWave("decision.wav");
#pragma endregion

	SceneManager::GetInstance()->SetUIFilename("TitleScene");
}

void TitleScene::Initialize()
{
	SceneManager::GetInstance()->SetIsActivePause(true);

	std::unique_ptr<ICamera> camera = std::make_unique<NormalCamera>();
	camera->Initialize(Vector3D(0.0f, 0.0f, -10.0f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	LoadResources();

#pragma region Sprite
	backSprite_ = std::make_unique<Sprite>();
	backSprite_->Initialize(TextureManager::GetInstance()->GetWhiteTexture());
	backSprite_->SetSize(Vector2D{ Window::sWIN_WIDTH,Window::sWIN_HEIGHT });
	backSprite_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
#pragma endregion
}

void TitleScene::Finalize()
{
	XAudioManager::GetInstance()->StopAllSound();
	XAudioManager::GetInstance()->DeleteAllSound();
}

void TitleScene::MatUpdate()
{
	//	背景
	backSprite_->Update();
}

void TitleScene::Update()
{
	if (InputManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_B)) {
		SceneManager::GetInstance()->SetNextScene("GAMESCENE");
	}

	MatUpdate();
}

void TitleScene::ImguiUpdate()
{
}

void TitleScene::DrawShadow() {}

void TitleScene::Draw()
{
	//	背景
	backSprite_->Draw();
}
