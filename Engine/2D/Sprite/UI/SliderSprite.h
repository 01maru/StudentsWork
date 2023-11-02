#pragma once
#include "UIComponent.h"
#include "Vector2D.h"
#include <string>

/**
* @file SliderSprite.h
* @brief スライダーに必要な画像やスプライトの設定と円の動きを実装するためのファイル
*/

#pragma region 前置宣言

class UISprite;
class Texture;

#pragma endregion

class SliderSprite :public UIComponent
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize() override;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;

private:
	//	線の最左端
	Vector2D startPos_;
	//	線の長さ
	float railLen_ = 0.0f;

	float minValue_ = 0.0f;
	float maxValue_ = 1.0f;
	float value_ = 0.0f;
	float spd_ = 0.01f;

	UISprite* sprites_ = nullptr;

#pragma region Texture

	Texture* circleTex_ = nullptr;
	Texture* railTex_ = nullptr;

#pragma endregion

public:
#pragma region Getter

	float GetValue();

#pragma endregion

#pragma region Setter

	void ValueUpdate(float& v, int32_t inputValue);
	void SetValue(float v);

	void SetRailLength(float len);

	void SetRailStartPos(const Vector2D& pos);
	/**
	* @fn SetRailTexture(const std::string&)
	* railの画像取得用関数(読み込まれていなかったら読み込みもする)
	* @param texName railの画像名(.png or .jpg)
	*/
	void SetRailTexture(const std::string& texName);
	/**
	* @fn SetCircleTexture(const std::string&)
	* circleの画像取得用関数(読み込まれていなかったら読み込みもする)
	* @param texName circleの画像名(.png or .jpg)
	*/
	void SetCircleTexture(const std::string& texName);

#pragma endregion
};

