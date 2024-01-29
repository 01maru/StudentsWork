#pragma once
#include "VertIdxBuff.h"
#include "ConstBuff.h"

#include "Texture.h"
#include "MyMath.h"
#include <vector>
#include <iostream>

/**
* @file Sprite.h
* @brief スプライト表示用ファイル
*/

namespace MNE
{

	namespace CBuff {
		struct CBuffColorMaterial;
		struct CBuffSpriteTransform;
	}
	class GPipeline;

	class Sprite :public VertIdxBuff
	{
	protected:
		static Matrix sMAT_2DTRANSFORM;

		uint16_t tag_ = 0b0000000000000000;
	
		//	画像を張り付けるポリゴンの設定
		MyMath::SpriteMatrix mat_;
		Vector2D size_ = { 100.0f,100.0f };
		Vector2D anchorPoint_;

		//	表示する画像の設定
		Vector2D textureLeftTop_;
		Vector2D textureSize_ = { 100.0f,100.0f };
		Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };
		bool isFlipX_ = false;
		bool isFlipY_ = false;

		Texture* handle_ = nullptr;

		bool isInvisible_ = false;

		//	dirtyFlag
		bool dirtyFlagUV_ = true;
		bool dirtyFlagPos_ = true;
		bool dirtyFlagMat_ = true;
		bool dirtyFlagColor_ = true;

	#pragma region VertBuff
		//	頂点情報
		std::vector<MNE::ScreenVertex> vertices_;

		enum VertexNumber {
			LB,
			LT,
			RB,
			RT,
			VertexSize,
		};
	#pragma endregion

	#pragma region CBuff
		//	行列
		MNE::ConstBuff cbTransform_;
		MNE::CBuff::CBuffSpriteTransform* cbTransformMat_ = nullptr;
		//	色
		MNE::ConstBuff cbColorMaterial_;
		MNE::CBuff::CBuffColorMaterial* cbMaterialMap_ = nullptr;
	#pragma endregion

	private:
		void SetVertices() override;

		void MatUpdate();
		//	verticesの座標設定
		void SetVerticesPos();
		//	verticesのUV設定
		void SetVerticesUV();
		//	vertices転送
		void TransferVertex();

		//	画像サイズを取得する
		void AdjustTextureSize();
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
		virtual void Draw(MNE::GPipeline* pipeline = nullptr);

	#pragma region Getter

		const Vector2D& GetPosition() const { return mat_.trans_; }
		const Vector2D& GetSize() const { return size_; }
		const Vector2D& GetAnchorPoint() const { return anchorPoint_; }
		float GetRotation() { return mat_.GetAngle(); }
		const Vector4D& GetColor() const { return color_; }
		const Vector2D& GetTextureSize() const { return textureSize_; }
		const Vector2D& GetTextureLeftTop() const { return textureLeftTop_; }

		Texture* GetTexture() { return handle_; }

		uint16_t GetTags() { return tag_; }

	#pragma endregion

	#pragma region Setter

		void SetPosition(const Vector2D& position);
		void SetRotation(float rotation);
	
		void SetAlphaColor(float alpha);
		void SetColor(const Vector4D& color);
		void SetColor(const Vector3D& color);
	
		void SetSize(const Vector2D& size);
		void SetAnchorPoint(const Vector2D& anchor);

		void SetTextureLeftTop(const Vector2D& leftTop);
		void SetTextureSize(const Vector2D& size);
		void SetTexture(Texture* handle);

		void SetTags(const uint16_t tag) { tag_ = tag; }

	#pragma endregion
	};

}
