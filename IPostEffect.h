#pragma once
#include "PlanePolygon.h"
#include "ConstBuff.h"
#include "ViewPortScissorRect.h"
#include "DepthStencil.h"
#include "Texture.h"
#include <string>
#include <vector>
#include <cstdint>

namespace MNE
{
#pragma region 前置宣言
	namespace CBuff {
		struct CBuffColorMaterial;
	}
	class GPipeline;
#pragma endregion

	
	class IPostEffect :public PlanePolygon
	{
	public:
		void Initialize(int32_t width, int32_t height, const std::string& name, int32_t textureNum = 1, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
		virtual void Update();
		/**
		* @fn Draw()
		* 描画処理関数
		*/
		virtual void Draw(int32_t mode = 0);

	protected:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region ConstBuff

		CBuff::CBuffColorMaterial* cMaterialMap_ = nullptr;
		ConstBuff material_;

#pragma endregion
		std::string name_;

		//	ビューポートシザー矩形
		ViewPortScissorRect viewPortSciRect_;
		ComPtr<ID3D12DescriptorHeap> rtvHeap_;
		DepthStencil dsv_;

		//	使用するテクスチャポインタ
		std::vector<Texture*> texture_;
		//	使用するパイプライン
		GPipeline* pipeline_ = nullptr;

		//	ポストエフェクト全体の色
		MyMath::Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };
		//	ポストエフェクトのサイズ
		int32_t width_ = 0;
		int32_t height_ = 0;
		//	クリアカラー
		MyMath::Vector4D clearColor_ = { 0.1f,0.25f, 0.5f,0.0f };

		int32_t mode_ = 0;

		IPostEffect* originalPE_ = nullptr;

	public:

#pragma region Getter

		int32_t GetWidth() const;
		int32_t GetHeight() const;
		const MyMath::Vector4D& GetClearColor();
		int32_t GetTextureNum();
		Texture* GetTexture(int32_t index = 0);
		std::string GetName();

#pragma endregion

#pragma region Setter

		void RSSetVPandSR();
		void SetGPipelineAndIAVertIdxBuff();
		void SetMode(int32_t mode);
		void SetGPipeline(GPipeline* pipeline);
		ID3D12Resource* GetTextureBuff(int32_t index = 0) { return texture_[index]->GetResourceBuff(); }
		ID3D12DescriptorHeap* GetRTVHeap() { return rtvHeap_.Get(); }
		ID3D12DescriptorHeap* GetDSVHeap() { return dsv_.GetDSVHeap(); }
		void SetColor(const MyMath::Vector4D& color);
		void SetClearColor(const MyMath::Vector4D& color);
		void SetOriginalPostEffect(IPostEffect* original);

#pragma endregion
	};

}
