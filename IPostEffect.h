#pragma once
#include "PlanePolygon.h"
#include "ConstBuff.h"
#include "ViewPortScissorRect.h"
#include "DepthStencil.h"
#include <string>

namespace MNE
{
#pragma region 前置宣言
	namespace CBuff {
		struct CBuffColorMaterial;
	}
	class Texture;
	class GPipeline;
#pragma endregion

	
	class IPostEffect :public PlanePolygon
	{
	public:
		void Initialize(int32_t width, int32_t height, const std::string& name, int32_t textureNum = 1, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
		void Update();
		/**
		* @fn Draw()
		* 描画処理関数
		*/
		virtual void Draw();

	protected:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region ConstBuff

		CBuff::CBuffColorMaterial* cMaterialMap_ = nullptr;
		ConstBuff material_;

#pragma endregion
		//	ビューポートシザー矩形
		ViewPortScissorRect viewPortSciRect_;
		ComPtr<ID3D12DescriptorHeap> rtvHeap_;
		DepthStencil dsv_;

		//	使用するテクスチャポインタ
		std::vector<Texture*> texture_;
		//	使用するパイプライン
		GPipeline* pipeline_;

		//	ポストエフェクト全体の色
		MyMath::Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };
		//	ポストエフェクトのサイズ
		int32_t width_ = 0;
		int32_t height_ = 0;
		//	クリアカラー
		MyMath::Vector4D clearColor_ = { 0.1f,0.25f, 0.5f,0.0f };

		typedef void (IPostEffect::* Original)();
		//	ポストエフェクト前のテクスチャの描画関数ポインタ
		Original original_;

	public:

#pragma region Getter

		int32_t GetWidth() const;
		int32_t GetHeight() const;
		const MyMath::Vector4D& GetClearColor();
		int32_t GetTextureNum();
		Texture* GetTexture(int32_t index = 0);
#pragma endregion

#pragma region Setter

		void RSSetVPandSR();
		void SetGPipelineAndIAVertIdxBuff();

		void SetColor(const MyMath::Vector4D& color);
		void SetClearColor(const MyMath::Vector4D& color);
		void SetOriginal(Original original);
#pragma endregion
	};

}
