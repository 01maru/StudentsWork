//#include "ActiveSkillEffect.h"
//#include "TextureManager.h"
//#include "Easing.h"
//
//using namespace Easing;
//using namespace MNE;
//
//void ActiveSkillEffect::Initialize()
//{
//	timer_.Initialize(effectTime_, true, true, MAX_COUNT);
//
//	Texture* tex = TextureManager::GetInstance()->GetTextureGraph("skillEffect.png");
//	texSize_ = tex->GetTextureSize();
//	Vector2D midBot(0.5f, 1.0f);
//
//	//	スプライト追加
//	for (int16_t i = 0; i < SPRITE_NUM; i++)
//	{
//		sprites_.emplace_back();
//		Sprite* sprite = &sprites_.back();
//		sprite->Initialize(tex);
//		sprite->SetAnchorPoint(midBot);
//	}
//}
//
//void ActiveSkillEffect::LoadResources()
//{
//	TextureManager::GetInstance()->LoadTextureGraph("skillEffect.png");
//}
//
//void ActiveSkillEffect::StartAnimeUpdate()
//{
//	if (timer_.GetIsIncrement() == FALSE) return;
//
//	for (int16_t i = 0; i < SPRITE_NUM; i++)
//	{
//		Vector2D size = sprites_[i].GetSize();
//		size.y = EaseIn(0.0f, moveSizeY_[i], timer_.GetCountPerMaxCount(), Double);
//		sprites_[i].SetSize(size);
//	}
//}
//
//void ActiveSkillEffect::EndAnimeUpdate()
//{
//	if (timer_.GetIsIncrement() == TRUE) return;
//
//	for (int16_t i = 0; i < SPRITE_NUM; i++)
//	{
//		Vector2D size = sprites_[i].GetSize();
//		size.x = EaseIn(0.0f, texSize_.x / 4.0f, timer_.GetCountPerMaxCount(), Double);
//		size.y = EaseIn(0.0f, moveSizeY_[i], timer_.GetCountPerMaxCount(), Double);
//		sprites_[i].SetSize(size);
//
//		//Vector2D pos = lerp(endPos_, startPos_, timer_.GetCountPerMaxCount());
//		//pos.x = sprites_[i].GetPosition().x;
//		//sprites_[i].SetPosition(pos);
//	}
//}
//
//void ActiveSkillEffect::TexScaleUpdate()
//{
//	for (auto& sprite : sprites_)
//	{
//		sprite.SetTextureSize(sprite.GetSize() * 2.0f);
//	}
//}
//
//void ActiveSkillEffect::Update()
//{
//	if (timer_.GetIsActive() == FALSE) return;
//	
//	timer_.Update();
//
//	StartAnimeUpdate();
//	EndAnimeUpdate();
//
//	TexScaleUpdate();
//
//	for (auto& sprite : sprites_)
//	{
//		sprite.Update();
//	}
//}
//
//void ActiveSkillEffect::Draw()
//{
//	if (timer_.GetIsActive() == FALSE) return;
//
//	for (auto& sprite : sprites_)
//	{
//		sprite.Draw();
//	}
//}
//
//void ActiveSkillEffect::Start()
//{
//	timer_.SetIsIncrement(true);
//	timer_.StartCount();
//
//	moveSizeY_.clear();
//	for (int16_t i = 0; i < SPRITE_NUM; i++)
//	{
//		//	開始位置
//		Vector2D pos = startPos_;
//		float randVMax = 2.0f * i;
//		pos.x += MyMath::GetRand(-randVMax, randVMax);
//		sprites_[i].SetPosition(pos);
//		//	横幅設定
//		Vector2D size;
//		size.x = texSize_.x / 4.0f;
//		sprites_[i].SetSize(size);
//
//		//Vector2D randTexLT = MyMath::GetRand(Vector2D(), texSize_);
//		//sprites_[i].SetTextureLeftTop(randTexLT);
//
//		//	伸びる
//		float moveSize = MyMath::mMin(maxSizeY_ / static_cast<float>(SPRITE_NUM) * (SPRITE_NUM - i), maxSizeY_ / 3.0f * 2.0f);
//		moveSizeY_.push_back(moveSize);
//	}
//}
