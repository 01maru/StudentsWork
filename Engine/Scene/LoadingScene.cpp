#include "LoadingScene.h"
#include "LoadingModel.h"
#include "CameraManager.h"
#include "ObjCamera2D.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Window.h"
#include "Easing.h"
#include "LoadingSprite.h"

#include "InputManager.h"

using namespace Easing;
using namespace MNE;
using namespace MyMath;

void LoadingScene::Initialize()
{
	LoadResources();

	counter_.Initialize(MAX_COUNT, true, true);

	std::unique_ptr<ICamera> camera = std::make_unique<ObjCamera2D>();
	CameraManager::GetInstance()->SetOrthoProjCamera(camera);

	Vector2D posGap(80, 80);

	loadSprite_ = std::make_unique<LoadSpriteAnime>();
	loadSprite_->Initialize(Vector2D(Window::sWIN_WIDTH, Window::sWIN_HEIGHT) - posGap, OBJ_MAX_COUNT);

	backSprite_ = std::make_unique<DissolveSprite>();
	backSprite_->Initialize();
	backSprite_->SetSize(Vector2D(Window::sWIN_WIDTH, Window::sWIN_HEIGHT));
	Vector3D whiteColor(0.8f, 0.8f, 0.8f);
	backSprite_->SetColor(whiteColor);

	texSize_ = TextureManager::GetInstance()->GetTextureGraph("noise.png")->GetTextureSize();
}

void LoadingScene::LoadResources()
{
	TextureManager::GetInstance()->LoadTextureGraph("noise.png");
}

void LoadingScene::Update()
{
	counter_.Update();
	isDraw_ = counter_.GetFrameCount() != 0;

	if (isDraw_ == false) return;

	float start = 0.0f;
	float end = 1.0f;
	float value = EaseOut(start, end, counter_.GetCountPerMaxCount(), backEasePow_);
	backSprite_->SetDissolveValue(value);

	backSprite_->Update();
	//loadObj_->Update();
	loadSprite_->Update();
}

void LoadingScene::Draw()
{
	if (isDraw_ == false) return;

	backSprite_->Draw();

	//loadObj_->Draw();

	loadSprite_->Draw();
}

void LoadingScene::SetIsLoading(bool loading)
{
	//loadObj_->SetIsLoading(loading);
	loadSprite_->SetIsLoading(loading);

	backSprite_->SetTextureLeftTop(MyMath::GetRand(Vector2D(), texSize_));
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
