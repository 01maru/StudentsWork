#pragma once
#include "Vector2D.h"
#include "Sprite.h"
#include <cstdint>

/**
* @file UIButton.h
* @brief UIEditor用のButtonをまとめたファイル
*/

class UIButton :public Sprite
{
private:
	Vector2D pos_;			//	中心位置
	//Vector2D size_;
	
	////bool isActive_ = false;
	bool isSelect_ = false;	//	選択中か

	////int16_t number_;
	int16_t priorityNum_;

public:
	/**
	* @fn EffectUpdate()
	* エフェクト更新処理関数
	*/
	void EffectUpdate();
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw();

	//	Getter
	////bool GetIsActive() { return isActive_; }
	////int16_t GetNumber() { return number_; }

#pragma region Setter

	void SetPos(const Vector2D& pos) { pos_ = pos; }
	//void SetSize(const Vector2D& size) { size_ = size; }
	////void SetIsActive(bool isActive) { isActive_ = isActive; }
	////void SetNumber(int16_t& index);

#pragma endregion
};

