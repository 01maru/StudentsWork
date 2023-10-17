#pragma once
#include "VertIdxBuff.h"
#include "ConstBuff.h"
#include "BlendMord.h"

/**
* @file IParticle.h
* @brief 1つのパーティクルの機能をまとめたファイル
*/

namespace CBuff {
	struct CBuffColorMaterial;
	struct CBufferParticleTransform;
}

class IParticle :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region CBuff

	ConstBuff transform_;
	CBuff::CBufferParticleTransform* cTransformMap_ = nullptr;

	ConstBuff colorMaterial_;
	CBuff::CBuffColorMaterial* cColorMap_ = nullptr;

#pragma endregion

protected:
	//	中心座標
	Vector3D vertex_;
	//	色
	Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };
	//	パーティクルのスケール
	float scale_ = 1.0f;
	//	ビルボード
	bool isBillboard_ = false;
	bool isBillboardY_ = false;
	//	画像のハンドル
	int32_t texHandle_ = -1;
	//	パーティクル終了フラグ
	bool isEnd_ = false;
	//	ブレンド設定
	Blend::BlendMord blendMord_ = Blend::ALPHA_BLEND;

private:
	/**
	* @fn SetVertices()
	* 頂点バッファ設定用関数
	*/
	void SetVertices() override;

protected:
	/**
	* @fn TransferVertex()
	* GPUのバッファに座標情報をMapする関数
	*/
	void TransferVertex();

public:
	IParticle() {};
	virtual ~IParticle() = default;

	/**
	* @fn Initialize()
	* 初期化処理関数
	*/
	void Initialize();
	/**
	* @fn Update()
	* 更新処理用関数
	*/
	virtual void Update() = 0;
	/**
	* @fn MatUpdate()
	* 行列更新用関数
	*/
	void MatUpdate();
	/**
	* @fn Draw()
	* 描画処理用関数
	*/
	void Draw();

#pragma region Getter

	/**
	* @fn GetIsEnd()
	* パーティクル消滅フラグのisEnd_のGetter関数
	* @return isEnd_の値
	*/
	bool GetIsEnd() { return isEnd_; }
	/**
	* @fn GetPosition()
	* vertex_のGetter関数
	* @return vertex_の値
	*/
	const Vector3D& GetPosition() { return vertex_; }
	/**
	* @fn GetColor()
	* color_のGetter関数
	* @return color_の値
	*/
	const Vector4D& GetColor() { return color_; }
	/**
	* @fn GetScale()
	* scale_のGetter関数
	* @return scale_の値
	*/
	float GetScale() { return scale_; }
	/**
	* @fn GetIsBillboard()
	* isBillboard_のGetter関数
	* @return isBillboard_の値
	*/
	bool GetIsBillboard() { return isBillboard_; }
	/**
	* @fn GetIsBillboardY()
	* isBillboardY_のGetter関数
	* @return isBillboardY_の値
	*/
	bool GetIsBillboardY() { return isBillboardY_; }
	/**
	* @fn GetBlendMord()
	* blendMord_のGetter関数
	* @return blendMord_
	*/
	Blend::BlendMord GetBlendMord() { return blendMord_; }

#pragma endregion

#pragma region Setter

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
	void SetColor(const Vector4D& color) { color_ = color; }
	/**
	* @fn SetScale(float)
	* scale_の値を変更するための関数
	* @param scale scale_の変更後の値
	*/
	void SetScale(float scale) { scale_ = scale; }
	/**
	* @fn SetIsBillboard(bool)
	* isBillboard_の値を変更するための関数
	* @param isBillboard isBillboard_の変更後の値
	*/
	void SetIsBillboard(bool isBillboard) { isBillboard_ = isBillboard; }
	/**
	* @fn SetIsBillboardY(bool)
	* isBillboardY_の値を変更するための関数
	* @param isBillboardY isBillboardY_の変更後の値
	*/
	void SetIsBillboardY(bool isBillboardY) { isBillboardY_ = isBillboardY; }

#pragma endregion
};

