#include "SliderSprite.h"
#include "TextureManager.h"
#include "UISprite.h"
#include "UIObject.h"

void SliderSprite::Initialize()
{
	sprites_ = parent_->GetComponent<UISprite>();

	//	UISpriteコンポーネントがなかったら追加
	if (sprites_ == nullptr) {
		sprites_ = parent_->AddComponent<UISprite>();
	}

	Sprite railSprite;
	railSprite.Initialize(railTex_);
	railSprite.SetAnchorPoint(Vector2D{ 0.0f,0.5f });
	sprites_->AddSprite("Rail", railSprite);

	Sprite circleSprite;
	circleSprite.Initialize(circleTex_);
	circleSprite.SetAnchorPoint(Vector2D{ 0.5f,0.5f });
	sprites_->AddSprite("Circle", circleSprite);
}

void SliderSprite::Update()
{
	Vector2D circlePos = startPos_;

	circlePos.x += value_ * railLen_;

	sprites_->GetSprites()["Circle"].SetPosition(circlePos);
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

float SliderSprite::GetValue()
{
	return value_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void SliderSprite::SetValue(float v)
{
	value_ = v;
}

void SliderSprite::SetRailLength(float len)
{
	railLen_ = len;
}

void SliderSprite::SetRailStartPos(const Vector2D& pos)
{
	startPos_ = pos;
}

void SliderSprite::SetRailTexture(const std::string& texName)
{
	railTex_ = TextureManager::GetInstance()->LoadTextureGraph(texName);
}

void SliderSprite::SetCircleTexture(const std::string& texName)
{
	circleTex_ = TextureManager::GetInstance()->LoadTextureGraph(texName);
}
