#include "SplashSprite.h"
#include "TextureManager.h"
#include "Window.h"
#include "PipelineManager.h"
#include "Easing.h"

using namespace MNE;

void SplashSprite::Initialize(Texture* texture)
{
	count_ = std::make_unique<FrameCounter>();
	count_->Initialize(30, true, true, 4);

	Sprite::Initialize(texture);
	
	mat_.SetTrans(Vector2D(Window::sWIN_WIDTH / 2.0f, Window::sWIN_HEIGHT / 2.0f));
	anchorPoint_ = Vector2D(0.5f, 0.5f);
}

void SplashSprite::Update()
{
	if (!count_->GetIsActive()) return;

	count_->Update();
	
	float color = 0.0f;
	if (count_->GetCount() == 1 || count_->GetCount() == 2) color = 1.0f;

	else color = Easing::lerp(0.0f, 1.0f, count_->GetCountPerMaxCount());

	color_.w = color;
	dirtyFlagColor_ = true;
	Sprite::Update();
}

void SplashSprite::Draw()
{
	if (!count_->GetIsActive()) return;

	Sprite::Draw();
}
