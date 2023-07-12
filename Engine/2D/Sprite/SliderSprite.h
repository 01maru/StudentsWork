//#pragma once
//#include "Sprite.h"
//
//class SliderSprite
//{
//private:
//	//	線の最左端
//	Vector2D startPos_;
//	//	線の長さ
//	float railLen_ = 0.0f;
//
//#pragma region Sprite
//	std::unique_ptr<Sprite> circleSprite_;
//	std::unique_ptr<Sprite> railSprite_;
//#pragma endregion
//
//#pragma region Texture
//	Texture* circleTex;
//	Texture* railTex;
//#pragma endregion
//
//public:
//	void Initialize();
//	//	valueは0.0~1.0の間の値
//	void Update(float value);
//	void Draw();
//
//	void SetColor(const Vector4D& color);
//	void SetSliderPos(const Vector2D& start, float len);
//
//	const Vector2D& GetCirclePos(float value) { return { startPos_.x + value * railLen_ ,startPos_.y }; }
//};
//
