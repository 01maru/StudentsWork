#include "SliderSprite.h"
#include "TextureManager.h"
#include "UISprite.h"
#include "UIObject.h"
#include "InputManager.h"

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

void MNE::SliderSprite::ColliderUpdate()
{
	Vector2D halfSize = size_ / 2.0f;
	Vector2D posLT = sprites_->GetSprites()["Circle"].GetPosition() - halfSize;
	Vector2D posRB = sprites_->GetSprites()["Circle"].GetPosition() + halfSize;
	Vector2D cursorPos = InputManager::GetInstance()->GetMouse()->GetCursor();
	if (MyMath::CollisionSquareToPoint(posLT, posRB, cursorPos) == TRUE)
	{
		if (select_ == FALSE)
		{
			if (InputManager::GetInstance()->GetMouse()->GetClickTrigger(InputMouse::LeftClick))
			{
				select_ = TRUE;
			}
		}
	}
	if (select_ == TRUE)
	{
		if (InputManager::GetInstance()->GetMouse()->GetClickRelease(InputMouse::LeftClick) ||
			InputManager::GetInstance()->GetUsePad() == TRUE)
		{
			select_ = FALSE;
		}
		else
		{
			if (InputManager::GetInstance()->GetMouse()->GetClick(InputMouse::LeftClick))
			{
				Vector2D railLeft = sprites_->GetSprites()["Rail"].GetPosition();
				Vector2D railRight = sprites_->GetSprites()["Rail"].GetPosition();
				railRight.x += railLen_;
				
				cursorPos.x = MyMath::mClamp(railLeft.x, railRight.x, cursorPos.x);
				cursorPos.y = startPos_.y;

				sprites_->GetSprites()["Circle"].SetPosition(cursorPos);

				value_ = (cursorPos.x - startPos_.x) / railLen_;

				InputManager::GetInstance()->GetMouse()->SetCursorPosition(cursorPos);
			}
		}
	}
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
	if (select_ == TRUE)
	{
		v = value_;
	}
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

void MNE::SliderSprite::SetRailTexture(const std::string& texName, int16_t tag)
{
	railTex_ = TextureManager::GetInstance()->LoadTextureGraph(texName);

	Sprite railSprite;
	railSprite.Initialize(railTex_);
	railSprite.SetAnchorPoint(Vector2D{ 0.0f,0.5f });
	railSprite.SetTags(tag);
	sprites_->AddSprite("Rail", railSprite);
}

void MNE::SliderSprite::SetCircleTexture(const std::string& texName, int16_t tag)
{
	circleTex_ = TextureManager::GetInstance()->LoadTextureGraph(texName);
	Sprite circleSprite;
	circleSprite.Initialize(circleTex_);
	circleSprite.SetAnchorPoint(Vector2D{ 0.5f,0.5f });
	circleSprite.SetSize(size_);
	circleSprite.SetTags(tag);
	sprites_->AddSprite("Circle", circleSprite);
}
