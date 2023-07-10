//#include "SliderSprite.h"
//#include "TextureManager.h"
//
//void SliderSprite::Initialize()
//{
//	circleSprite_ = std::make_unique<Sprite>(circleTex);
//	circleSprite_->SetSize(Vector2D{ 28, 28 });
//	circleSprite_->SetAnchorPoint(Vector2D{ 0.5f,0.5f });
//	circleSprite_->SetColor({ 1.2f,1.2f,1.2f,1.0f });
//
//	railSprite_ = std::make_unique<Sprite>(railTex);
//	railSprite_->SetAnchorPoint(Vector2D{ 0.0f,0.5f });
//	railSprite_->SetColor({ 1.2f,1.2f,1.2f,1.0f });
//
//	circleTex = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/sliderBalUIl.png");
//	railTex = TextureManager::GetInstance()->LoadTextureGraph(L"Resources/Sprite/sliderBarUI.png");
//}
//
//void SliderSprite::Update(float value)
//{
//	Vector2D circlePos = startPos_;
//
//	circlePos.x += value * railLen_;
//
//	circleSprite_->SetPosition(circlePos);
//	
//	circleSprite_->Update();
//	railSprite_->Update();
//}
//
//void SliderSprite::Draw()
//{	
//	railSprite_->Draw();
//	circleSprite_->Draw();
//}
//
//void SliderSprite::SetColor(const Vector4D& color)
//{
//	railSprite_->SetColor(color);
//	circleSprite_->SetColor(color);
//}
//
//void SliderSprite::SetSliderPos(const Vector2D& start, float len)
//{
//	startPos_ = start;
//	railLen_ = len;
//
//	railSprite_->SetSize(Vector2D{ len, 10.0f });
//	railSprite_->SetPosition(startPos_);
//}
