#pragma once
#include "UIComponent.h"
#include "Vector2D.h"

class UIAnimationTimer;
class UISprite;

class UIMoveAnimation :public UIComponent
{
private:
	UISprite* sprite_ = nullptr;
	UIAnimationTimer* timer_ = nullptr;
	Vector2D startPos;
	Vector2D endPos;

public:
	void Initialize() override;
	void Update() override;

	void SetStartPos(const Vector2D& pos) { startPos = pos; }
	void SetEndPos(const Vector2D& pos) { endPos = pos; }

	Vector2D& GetStartPos() { return startPos; }
	Vector2D& GetEndPos() { return endPos; }
};

