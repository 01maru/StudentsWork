#include "LoadSpriteAnime.h"
#include "TextureManager.h"

using namespace MNE;

void LoadSpriteAnime::Initialize(const Vector2D& centerPos, int32_t fadeTime)
{
	counter_.SetIsEndless(true);

	Vector2D centerPoint(0.5f, 0.5f);
	std::vector<Vector2D> pos;

	pos.push_back(centerPos - Vector2D(spriteSize_, -spriteSize_));

	pos.push_back(centerPos - Vector2D(spriteSize_, 0.0f));
	pos.push_back(centerPos + Vector2D(0.0f, spriteSize_));

	pos.push_back(centerPos - Vector2D(spriteSize_, spriteSize_));
	pos.push_back(centerPos);
	pos.push_back(centerPos + Vector2D(spriteSize_, spriteSize_));

	pos.push_back(centerPos - Vector2D(0.0f, spriteSize_));
	pos.push_back(centerPos + Vector2D(spriteSize_, 0.0f));

	pos.push_back(centerPos - Vector2D(-spriteSize_, spriteSize_));

	for (int32_t i = 0; i < pos.size(); i++)
	{
		LoadingSprite sprite;
		sprite.Initialize();
		sprite.SetFadeAnimeTime(fadeTime);
		sprite.SetTexture(TextureManager::GetWhiteTexture());
		sprite.SetAnchorPoint(centerPoint);
		sprite.SetPosition(pos[i]);
		sprite.SetMaxSize(spriteSize_);
		sprites_.push_back(sprite);
	}
}

void LoadSpriteAnime::ActiveUpdate()
{
	if (counter_.GetFrameCount() % TIME == 0) {
		rowNum_ = MyMath::mMin(counter_.GetFrameCount() / TIME, MAX_ROW_NUM);
		if (rowNum_ >= MAX_ROW_NUM)	return;

		for (int32_t i = index_[rowNum_]; i < index_[rowNum_ + 1]; i++)
		{
			sprites_[i].Start();
		}
	}
}

void LoadSpriteAnime::Update()
{
	ActiveUpdate();

	counter_.Update();

	for (int32_t i = 0; i < sprites_.size(); i++)
	{
		sprites_[i].Update();
	}
}

void LoadSpriteAnime::Draw()
{
	for (int32_t i = 0; i < sprites_.size(); i++)
	{
		sprites_[i].Draw();
	}
}

void LoadSpriteAnime::Reset()
{
	counter_.StartCount();
}

void LoadSpriteAnime::SetIsLoading(bool loading)
{
	for (int32_t i = 0; i < sprites_.size(); i++)
	{
		sprites_[i].SetIsLoading(loading);
	}

	if (loading == true)
	{
		int32_t time = TIME * MAX_ROW_NUM + WAIT_TIME;
		counter_.Initialize(time, true);
		counter_.StartCount();

		for (int32_t i = 0; i < sprites_.size(); i++)
		{
			sprites_[i].Reset();
		}
	}
}
