#include "SplashScreenScene.h"
#include "Window.h"
#include "TextureManager.h"

#include "PipelineManager.h"

void SplashScreenScene::Initialize()
{
	count_ = std::make_unique<FrameCounter>();
	count_->Initialize(60 + 120, true);
	count_->StartCount();

	LoadResources();

	backSprite_ = std::make_unique<Sprite>();
	backSprite_->Initialize();
	backSprite_->SetSize(Vector2D(Window::sWIN_WIDTH, Window::sWIN_HEIGHT));
	backSprite_->SetColor(Vector4D(0.0f, 0.0f, 0.0f, 1.0f));

	engineSprite_ = std::make_unique<SplashSprite>();
	engineSprite_->Initialize(TextureManager::GetInstance()->LoadTextureGraph("rogo.png"));
}

void SplashScreenScene::Finalize()
{
}

void SplashScreenScene::LoadResources()
{
}

void SplashScreenScene::Update()
{
	count_->Update();

	if (count_->GetFrameCount() == 60) engineSprite_->ActiveCounter();

	backSprite_->Update();
	engineSprite_->Update();
}

void SplashScreenScene::ImguiUpdate()
{
}

void SplashScreenScene::Draw()
{
	backSprite_->Draw();
	engineSprite_->Draw();
}
