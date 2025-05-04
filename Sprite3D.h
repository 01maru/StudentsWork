#pragma once
#include "VertIdxBuff.h"
#include "Texture.h"
#include "ConstBuff.h"

namespace MNE
{
	namespace CBuff {
		struct CBuffObjColorMaterial;
		struct CBuffObj3DTransform;
	}

	class GPipeline;

	class Sprite3D :public VertIdxBuff
	{
	private:
		//	画像を張り付けるポリゴンの設定
		MyMath::ObjMatrix mat_;
		MyMath::Vector2D size_ = { 100.0f,100.0f };
		MyMath::Vector2D anchorPoint_;

		bool isFlipX_ = false;
		bool isFlipY_ = false;
		bool isBillboard_ = false;

		Texture* handle_ = nullptr;

		GPipeline* pipeline_ = nullptr;

#pragma region VertBuff
		//	頂点情報
		std::vector<ScreenVertex> vertices_;
		std::vector<uint16_t> indices_;

		enum VertexNumber {
			LB = 0,
			LT,
			RB,
			RT,
			VertexSize,
		};
#pragma endregion

#pragma region CBuff
		//	行列
		ConstBuff cbTransform_;
		CBuff::CBuffObj3DTransform* cbTransformMap_ = nullptr;
		//	色
		ConstBuff cbColorMaterial_;
		CBuff::CBuffObjColorMaterial* cbMaterialMap_ = nullptr;
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
		* @fn Draw()
		* 描画処理関数
		*/
		virtual void Draw();

	private:
		void SetVertices() override;

		void SetVerticesUV();
		void SetVerticesPos();
		//	画像サイズを取得する
		void AdjustTextureSize();
	};

}
