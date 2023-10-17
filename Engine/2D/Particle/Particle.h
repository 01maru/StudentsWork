#pragma once
#include "MyMath.h"
#include "VertIdxBuff.h"
#include "ConstBuff.h"

/**
* @file Particle.h
* @brief パーティクル一つの機能をまとめたファイル
*/

namespace CBuff {
	struct CBuffColorMaterial;
	struct CBufferParticleTransform;
}

class Particle :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region CBuff

	ConstBuff transform_;
	CBuff::CBufferParticleTransform* cTransformMap_ = nullptr;

	ConstBuff colorMaterial_;
	CBuff::CBuffColorMaterial* cColorMap_ = nullptr;

#pragma endregion

	Vector3D vertex_;
	Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };

	float scale_ = 1.0f;

	bool isBillboard_ = false;
	bool isBillboardY_ = false;

private:
	void SetVertices() override;
	void TransferVertex();

public:
	Particle();
	Particle(const Vector3D& pos);

	/**
	* @fn Initialize()
	* 初期化処理関数
	*/
	void Initialize();
	/**
	* @fn MatUpdate()
	* 行列更新用関数
	*/
	void MatUpdate();
	/**
	* @fn Draw(int32_t)
	* 描画処理用関数
	*/
	void Draw(int32_t handle);

#pragma region Getter

	const Vector3D& GetPosition() { return vertex_; }
	const Vector4D& GetColor() { return color_; }
	float GetScale() { return scale_; }
	bool IsBillboard() { return isBillboard_; }
	bool IsBillboardY() { return isBillboardY_; }

#pragma endregion

#pragma region Setter

	void SetPosition(const Vector3D& pos);
	void SetColor(const Vector4D& color) { color_ = color; }
	void SetScale(float scale);
	void SetIsBillboard(bool flag) { isBillboard_ = flag; }
	void SetIsBillboardY(bool flag) { isBillboardY_ = flag; }

#pragma endregion

	void Move(const Vector3D& spd);
};

