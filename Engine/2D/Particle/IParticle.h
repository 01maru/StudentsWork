#pragma once
#include "VertIdxBuff.h"
#include "ConstBuff.h"

/**
* @file IParticle.h
* @brief パーティクル一つの機能をまとめたファイル
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
	Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };

	float scale_ = 1.0f;

	bool isBillboard_ = false;
	bool isBillboardY_ = false;

	int32_t texHandle_ = -1;

	//	パーティクル終了フラグ
	bool isEnd_ = false;

private:
	void SetVertices() override;
protected:
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
	* @fn Draw(int32_t)
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
	const Vector4D& GetColor() { return color_; }
	float GetScale() { return scale_; }
	bool IsBillboard() { return isBillboard_; }
	bool IsBillboardY() { return isBillboardY_; }

#pragma endregion

#pragma region Setter

	void SetPosition(const Vector3D& pos);
	void SetColor(const Vector4D& color) { color_ = color; }
	void SetScale(float scale) { scale_ = scale; }
	void SetIsBillboard(bool flag) { isBillboard_ = flag; }
	void SetIsBillboardY(bool flag) { isBillboardY_ = flag; }

#pragma endregion
};

