#pragma once
#include "ParticleComponent.h"
#include "VertIdxBuff.h"
#include "ConstBuff.h"
#include "BlendMord.h"
#include "FrameCounter.h"

//	SpriteParticleに変更

/**
* @file Particle.h
* @brief 1つのペラパーティクルの機能をまとめたファイル
*/

#pragma region 前置宣言

namespace CBuff {
	struct CBuffColorMaterial;
	struct CBufferParticleTransform;
}

#pragma endregion

class SpriteParticle :public VertIdxBuff, public ParticleComponent
{
public:
	/**
	* @fn Initialize()
	* 初期化処理関数
	*/
	void Initialize() override;
	/**
	* @fn MatUpdate()
	* 座標更新処理関数
	*/
	void MatUpdate() override;
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw() override;

private:
#pragma region CBuff

	//	座標
	ConstBuff transform_;
	CBuff::CBufferParticleTransform* cTransformMap_ = nullptr;
	//	色
	ConstBuff colorMaterial_;
	CBuff::CBuffColorMaterial* cColorMap_ = nullptr;

#pragma endregion

	//	中心座標
	Vector3D vertex_;
	//	色
	Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };
	//	パーティクルのスケール
	float scale_ = 1.0f;
	//	ビルボード
	bool isBillboard_ = true;
	bool isBillboardY_ = false;
	//	画像のハンドル
	int32_t texHandle_ = -1;

public:
#pragma region Getter

	/**
	* @fn GetPosition()
	* vertex_のGetter関数
	* @return vertex_の値
	*/
	const Vector3D& GetPosition();
	/**
	* @fn GetColor()
	* color_のGetter関数
	* @return color_の値
	*/
	const Vector4D& GetColor();
	/**
	* @fn GetScale()
	* scale_のGetter関数
	* @return scale_の値
	*/
	float GetScale();
	/**
	* @fn GetIsBillboard()
	* isBillboard_のGetter関数
	* @return isBillboard_の値
	*/
	bool GetIsBillboard();
	/**
	* @fn GetIsBillboardY()
	* isBillboardY_のGetter関数
	* @return isBillboardY_の値
	*/
	bool GetIsBillboardY();

#pragma endregion

#pragma region Setter

	void SetTextureHandle(int32_t handle);
	/**
	* @fn SetPosition(const Vector3D&)
	* 中心座標の値を変更し、GPUに転送する関数
	* @param pos 中心座標の変更後の値
	*/
	void SetPosition(const Vector3D& pos);
	/**
	* @fn SetColor(const Vector4D&)
	* color_の値を変更するための関数
	* @param color color_の変更後の値
	*/
	void SetColor(const Vector4D& color);
	void SetColor(const Vector3D& color);
	void SetAlphaColor(float alpha);
	/**
	* @fn SetScale(float)
	* scale_の値を変更するための関数
	* @param scale scale_の変更後の値
	*/
	void SetScale(float scale);
	/**
	* @fn SetIsBillboard(bool)
	* isBillboard_の値を変更するための関数
	* @param isBillboard isBillboard_の変更後の値
	*/
	void SetIsBillboard(bool isBillboard);
	/**
	* @fn SetIsBillboardY(bool)
	* isBillboardY_の値を変更するための関数
	* @param isBillboardY isBillboardY_の変更後の値
	*/
	void SetIsBillboardY(bool isBillboardY);
	/**
	* @fn TransferVertex()
	* GPUのバッファに座標情報をMapする関数
	*/
	void TransferVertex();

private:
	/**
	* @fn SetVertices()
	* 頂点バッファ設定用関数
	*/
	void SetVertices() override;

#pragma endregion
};

