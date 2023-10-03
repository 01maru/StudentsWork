#pragma once
#include "Vector2D.h"
#include "Sprite.h"
#include <cstdint>

class UIButton
{
private:
	Vector2D pos_;			//	中心位置
	Vector2D size_;			//	サイズ
	
	bool isActive_ = false;	//	表示中か
	bool isSelect_ = false;	//	選択中か

	int16_t number_;		//	Active時の順番

	Sprite* pSprite_ = nullptr;
public:
	void Update();

	//	Getter
	bool GetIsActive() { return isActive_; }
	int16_t GetNumber() { return number_; }

	//	Setter
	void SetPos(const Vector2D& pos) { pos_ = pos; }
	void SetSize(const Vector2D& size) { size_ = size; }
	void SetSprite(Sprite* pSprite) { pSprite_ = pSprite; }
	void SetIsActive(bool isActive) { isActive_ = isActive; }
	void SetNumber(int16_t& index);
};

