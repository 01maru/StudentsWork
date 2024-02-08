#include "LoadSpriteAnime.h"
#include "TextureManager.h"

using namespace MNE;
using namespace MyMath;

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void LoadSpriteAnime::Initialize(const MyMath::Vector2D& centerPos, int32_t fadeTime)
{
	//	カウンターがアクティブだったら、カウントし続けるように
	counter_.SetIsEndless(TRUE);
	//	初期化
	int32_t time = TIME_BETWEEN_ANIME * MAX_ROW_NUM + WAIT_TIME;
	counter_.Initialize(time, true);

	//	9毎のスプライトをセット(左下から右上方向の順に格納)
	Vector2D centerPoint(0.5f, 0.5f);
	std::vector<Vector2D> pos;
	Vector2D offset;

	//	左下
	offset = Vector2D(-spriteSize_, spriteSize_);
	pos.push_back(centerPos + offset);

	//	左
	offset = Vector2D(-spriteSize_, 0.0f);
	pos.push_back(centerPos + offset);
	//	下
	offset = Vector2D(0.0f, spriteSize_);
	pos.push_back(centerPos + offset);

	//	左上
	offset = Vector2D(-spriteSize_, -spriteSize_);
	pos.push_back(centerPos + offset);
	//	真ん中
	pos.push_back(centerPos);
	//	右下
	offset = Vector2D(spriteSize_, spriteSize_);
	pos.push_back(centerPos + offset);

	//	上
	offset = Vector2D(0.0f, -spriteSize_);
	pos.push_back(centerPos + offset);
	//	右
	offset = Vector2D(spriteSize_, 0.0f);
	pos.push_back(centerPos + Vector2D(spriteSize_, 0.0f));

	//	右上
	offset = Vector2D(spriteSize_, -spriteSize_);
	pos.push_back(centerPos + offset);

	//	スプライト初期化
	for (auto itr : pos)
	{
		LoadingSprite sprite;
		sprite.Initialize();
		sprite.SetFadeAnimeTime(fadeTime);
		sprite.SetTexture(TextureManager::GetWhiteTexture());
		sprite.SetAnchorPoint(centerPoint);
		sprite.SetPosition(itr);
		sprite.SetMaxSize(spriteSize_);
		sprites_.push_back(sprite);
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void LoadSpriteAnime::ActiveUpdate()
{
	//	アニメーション開始時刻じゃなかったら処理しない
	if (counter_.GetFrameCount() % TIME_BETWEEN_ANIME != 0)	return;

	//	現在何番目の行か
	int32_t nowRowNum = MyMath::mMin(counter_.GetFrameCount() / TIME_BETWEEN_ANIME, MAX_ROW_NUM);

	//	現在の行が最大値以上だったら以下処理しない
	if (nowRowNum >= MAX_ROW_NUM)	return;

	//	スプライトのアニメーション開始
	for (int32_t i = index_[nowRowNum]; i < index_[nowRowNum + 1]; i++)
	{
		sprites_[i].Start();
	}
}

void LoadSpriteAnime::Update()
{
	//	スプライトのアニメーション開始フラグの更新
	ActiveUpdate();

	//	カウンター更新
	counter_.Update();

	//	スプライト更新
	for (auto& itr : sprites_)
	{
		itr.Update();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void LoadSpriteAnime::Draw()
{
	for (auto& itr : sprites_)
	{
		itr.Draw();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void LoadSpriteAnime::Reset()
{
	//	カウントスタート
	counter_.StartCount();
}

void LoadSpriteAnime::SetIsLoading(bool loading)
{
	//	スプライトそれぞれローディング中か設定
	for (auto& itr : sprites_)
	{
		itr.SetIsLoading(loading);
	}

	//	ローディング中だったらタイマーリセット
	if (loading == TRUE)
	{
		counter_.SetIsIncrement(TRUE);
		counter_.StartCount();

		for (auto& itr : sprites_)
		{
			itr.Reset();
		}
	}
}
