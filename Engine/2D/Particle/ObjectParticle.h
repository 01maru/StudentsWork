//#pragma once
//#include "Particle.h"
//#include "MyMath.h"
//#include "ConstBuff.h"
//
//namespace CBuff {
//	struct CBuffLightMaterial;
//	struct CBuffObj3DTransform;
//	struct CBuffSkinData;
//	struct CBuffColorMaterial;
//	struct CBuffDissolveData;
//}
//class IModel;
//
//class ObjectParticle :public Particle
//{
//public:
//	/**
//	* @fn Initialize()
//	* 初期化処理関数
//	*/
//	void Initialize();
//	/**
//	* @fn MatUpdate()
//	* 座標更新処理関数
//	*/
//	void MatUpdate();
//	/**
//	* @fn Draw()
//	* 描画処理関数
//	*/
//	void Draw();
//
//private:
//#pragma region CBuff
//
//	ConstBuff transform_;
//	CBuff::CBuffObj3DTransform* cTransformMap_ = nullptr;
//
//	ConstBuff colorMaterial_;
//	CBuff::CBuffColorMaterial* cColorMap_ = nullptr;
//
//	ConstBuff shadowTransform_;
//	CBuff::CBuffObj3DTransform* cShadowTransMap_ = nullptr;
//
//	ConstBuff lightMaterial_;
//	CBuff::CBuffLightMaterial* cLightMap_ = nullptr;
//
//	ConstBuff skinData_;
//	CBuff::CBuffSkinData* cSkinMap_ = nullptr;
//
//	ConstBuff dissolve_;
//	CBuff::CBuffDissolveData* cDissolveMap_ = nullptr;
//
//#pragma endregion
//
//	MyMath::ObjMatrix mat_;
//	Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };
//	IModel* model_ = nullptr;
//
//public:
//#pragma region Getter
//
//	const Vector4D& GetColor();
//	const Vector3D& GetPosition() { return mat_.trans_; }
//	const Vector3D& GetScale() { return mat_.scale_; }
//	const Vector3D& GetRotation() { return mat_.angle_; }
//	inline IModel* GetModel() { return model_; }
//	const Matrix& GetMatWorld() { return mat_.matWorld_; }
//
//#pragma endregion
//
//#pragma region Setter
//
//	void SetModel(IModel* model);
//	void SetColor(const Vector4D& color) { color_ = color; }
//	void SetColor(const Vector3D& color) { color_ = Vector4D(color, color_.w); }
//	void SetPosition(const Vector3D& pos) { mat_.trans_ = pos; }
//	void SetScale(const Vector3D& scale) { mat_.scale_ = scale; }
//	void SetRotation(const Vector3D& rot) { mat_.angle_ = rot; }
//
//#pragma endregion
//};
//
