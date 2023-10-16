#pragma once
#include "Sprite.h"

/**
* @file DissolveSprite.h
* @brief スプライトの表示にディゾルブを使うためのファイル
*/

namespace CBuff {
	struct CBuffDissolveData;
}

class DissolveSprite :public Sprite
{
private:
	bool dirtyFlagDissolve_ = true;
	float disolveValue_ = 0.0f;
	Vector4D dissolveColor_;

#pragma region CBuff
	ConstBuff cbDissolve_;
	CBuff::CBuffDissolveData* cbDissolveMat_ = nullptr;
#pragma endregion

public:
	/**
	* @fn Initialize(Texture*)
	* 初期化用関数
	* @param texture 表示する画像(何も設定しなかったら白色画像になる)
	*/
	void Initialize(Texture* texture = nullptr);
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();
	/**
	* @fn Draw(GPipeline*)
	* 描画処理関数
	*/
	void Draw(GPipeline* pipeline = nullptr) override;

#pragma region Getter

	/**
	* @fn GetDissolveValue()
	* disolveValue_のGetter関数
	* @return disolveValue_の値
	*/
	float const GetDissolveValue() { return disolveValue_; }
	/**
	* @fn GetDissolveColor()
	* dissolveColor_のGetter関数
	* @return dissolveColor_の値
	*/
	Vector4D& GetDissolveColor() { return dissolveColor_; }

#pragma endregion

#pragma region Setter

	/**
	* @fn SetDissolveValue(float)
	* disolveValue_の値を変更するための関数
	* @param value disolveValue_の変更後の値
	*/
	void SetDissolveValue(float value);

#pragma endregion
};

