#include "SliderSprite.h"
#include "TextureManager.h"
#include "UISprite.h"
#include "UIObject.h"

void MNE::SliderSprite::Initialize()
{
	sprites_ = parent_->GetComponent<UISprite>();

	//	UISpriteコンポーネントがなかったら追加
	if (sprites_ == nullptr) {
		sprites_ = parent_->AddComponent<UISprite>();
	}
}

void MNE::SliderSprite::Update()
{
	Vector2D circlePos = startPos_;

	circlePos.x += value_ * railLen_;

	sprites_->GetSprites()["Circle"].SetPosition(circlePos);
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

float MNE::SliderSprite::GetValue()
{
	return value_;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::SliderSprite::ValueUpdate(float& v, int32_t inputValue)
{
	v += inputValue * spd_;
	v = MyMath::mClamp(minValue_, maxValue_, v);
	value_ = v;
}

void MNE::SliderSprite::SetValue(float v)
{
	value_ = v;
}

void MNE::SliderSprite::SetRailLength(float len)
{
	sprites_->GetSprites()["Rail"].SetSize({ len,5.0f });
	railLen_ = len;
}

void MNE::SliderSprite::SetRailStartPos(const Vector2D& pos)
{
	sprites_->GetSprites()["Circle"].SetPosition(pos);
	sprites_->GetSprites()["Rail"].SetPosition(pos);
	startPos_ = pos;
}

void MNE::SliderSprite::SetRailTexture(const std::string& texName)
{
	railTex_ = TextureManager::GetInstance()->LoadTextureGraph(texName);

	Sprite railSprite;
	railSprite.Initialize(railTex_);
	railSprite.SetAnchorPoint(Vector2D{ 0.0f,0.5f });
	sprites_->AddSprite("Rail", railSprite);
}

void MNE::SliderSprite::SetCircleTexture(const std::string& texName)
{
	circleTex_ = TextureManager::GetInstance()->LoadTextureGraph(texName);
	Sprite circleSprite;
	circleSprite.Initialize(circleTex_);
	circleSprite.SetAnchorPoint(Vector2D{ 0.5f,0.5f });
	circleSprite.SetSize(Vector2D{ 10,10 });
	sprites_->AddSprite("Circle", circleSprite);
}
