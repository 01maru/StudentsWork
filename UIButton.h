#pragma once
#include "Vector2D.h"
#include "Sprite.h"
#include <cstdint>

class UIButton :public Sprite
{
private:
	Vector2D pos_;			//	���S�ʒu
	//Vector2D size_;			//	�T�C�Y
	
	////bool isActive_ = false;	//	�\������
	bool isSelect_ = false;	//	�I�𒆂�

	////int16_t number_;		//	Active���̏���

public:
	void EffectUpdate();
	void Draw();

	//	Getter
	////bool GetIsActive() { return isActive_; }
	////int16_t GetNumber() { return number_; }

	//	Setter
	void SetPos(const Vector2D& pos) { pos_ = pos; }
	//void SetSize(const Vector2D& size) { size_ = size; }
	////void SetIsActive(bool isActive) { isActive_ = isActive; }
	////void SetNumber(int16_t& index);
};
