#pragma once
#include "ViewPortScissorRect.h"
#include "ConstBuff.h"
#include "Texture.h"
#include <vector>
#include <cstdint>
#include "PlanePolygon.h"
#include "DepthStencil.h"

/**
* @file PostEffect.h
* @brief 板ポリ描画のポストエフェクト用ファイル
*/

namespace MNE
{

	namespace CBuff {
		struct CBuffColorMaterial;
	}
	class GPipeline;

	class PostEffect :public PlanePolygon
	{
	public:
		void Initialize(int32_t width, int32_t height, const std::string& name, int32_t textureNum = 1, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);
		/**
		* @fn Draw()
		* 描画処理関数
		*/
		virtual void Draw();

	private:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		std::vector<Texture*> texture_;
	
	#pragma region ConstBuff

		MNE::CBuff::CBuffColorMaterial* cMaterialMap_ = nullptr;
		MNE::ConstBuff material_;
	
	#pragma endregion

		std::string name_;

		MyMath::Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };
		int32_t width_ = 0;
		int32_t height_ = 0;
		MyMath::Vector4D clearColor_ = { 0.1f,0.25f, 0.5f,0.0f };

		ComPtr<ID3D12DescriptorHeap> rtvHeap_;
		//	ビューポートシザー矩形
		ViewPortScissorRect viewPortSciRect_;

		DepthStencil dsv_;
	public:

		void RSSetVPandSR();
		void SetGPipelineAndIAVertIdxBuff(MNE::GPipeline* pipeline);
		/**
		* @fn DrawLuminnce()
		* 描画処理関数
		*/
		void DrawLuminnce();
		/**
		* @fn Draw(GPipeline* , bool , int32_t)
		* 描画処理関数
		*/
		void Draw(MNE::GPipeline* pipeline, bool shadow, int32_t handle1 = -1);
		/**
		* @fn DrawDoF()
		* 描画処理関数
		*/
		void DrawDoF();
		/**
		* @fn DrawTask()
		* 描画処理関数
		*/
		void DrawTask();
		/**
		* @fn DrawMultiTask()
		* 描画処理関数
		*/
		void DrawMultiTask();

		void SetColor(const MyMath::Vector4D& color);

	#pragma region Getter

		int32_t GetWidth() const { return width_; }
		int32_t GetHeight() const { return height_; }
		const MyMath::Vector4D& GetClearColor() { return clearColor_; }
		const std::string& GetName() { return name_; }

		ID3D12Resource* GetTextureBuff(int32_t index = 0) { return texture_[index]->GetResourceBuff(); }
		ID3D12Resource** GetTextureBuffPtr(int32_t index = 0) { return texture_[index]->GetResourceBuffAddress(); }
		int32_t GetTextureNum() { return texture_.size(); }
		ID3D12DescriptorHeap* GetRTVHeap() { return rtvHeap_.Get(); }
		ID3D12DescriptorHeap* GetDSVHeap() { return dsv_.GetDSVHeap(); }
		Texture* GetTexture(int32_t index = 0);

	#pragma endregion
	};

}
