#include "LoadingScene.h"
#include "LoadingModel.h"
#include "CameraManager.h"
#include "ObjCamera2D.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Easing.h"

void LoadingScene::Initialize()
{
	LoadResources();

	counter_.Initialize(60, true, true);

	std::unique_ptr<ICamera> camera = std::make_unique<ObjCamera2D>();
	CameraManager::GetInstance()->SetOrthoProjCamera(camera);

	loadObj_ = std::make_unique<LoadingModel>();
	LoadingModel* object = dynamic_cast<LoadingModel*>(loadObj_.get());
	loadObj_->Initialize();
	loadObj_->SetFadeAnimeTime(30);
	object->SetModel(ModelManager::GetInstance()->GetModel("plane"));
	object->SetColor({ 1.0f,1.0f,1.0f });
	object->SetScale({ 30.0f,30.0f,30.0f });
	object->SetPosition({ Window::sWIN_WIDTH / 2.0f - 130.0f,-Window::sWIN_HEIGHT / 2.0f + 80.0f,0.0f });

	backSprite_ = std::make_unique<DissolveSprite>();
	backSprite_->Initialize();
	backSprite_->SetSize(Vector2D(Window::sWIN_WIDTH, Window::sWIN_HEIGHT));
	backSprite_->SetColor(Vector4D(0.8f, 0.8f, 0.8f, 1.0f));
}

void LoadingScene::LoadResources()
{
	ModelManager::GetInstance()->LoadModel("plane");

	TextureManager::GetInstance()->LoadTextureGraph("noise.png");
}

void LoadingScene::Update()
{
	counter_.Update();
	isDraw_ = counter_.GetFrameCount() != 0;

	if (isDraw_ == false) return;

	float value = Easing::EaseOut(0.0f, 1.0f, counter_.GetCountPerMaxCount(), 2);
	backSprite_->SetDissolveValue(value);

	backSprite_->Update();
	loadObj_->Update();
}

void LoadingScene::Draw()
{
	if (isDraw_ == false) return;

	backSprite_->Draw();

	loadObj_->Draw();
}

void LoadingScene::SetIsLoading(bool loading)
{
	loadObj_->SetIsLoading(loading);
}

bool LoadingScene::GetIsDrawn()
{
	return counter_.GetFrameCount() == counter_.GetMaxFrameCount();
}

void LoadingScene::StartFadeAnimation(bool isFadeIn)
{
	counter_.SetIsIncrement(isFadeIn);
	counter_.StartCount();
}
