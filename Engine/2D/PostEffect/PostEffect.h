#pragma once
#include "ViewPortScissorRect.h"
#include "ConstBuff.h"
#include "Texture.h"
#include <vector>
#include <cstdint>
#include "PlanePolygon.h"
#include "DepthStencil.h"

class GPipeline;
namespace CBuff {
	struct CBuffColorMaterial;
	struct CBuffGlayScale;
}

/**
* @file PostEffect.h
* @brief 板ポリ描画のポストエフェクト用ファイル
*/

class PostEffect :public PlanePolygon
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	int32_t texNum_ = 2;
	std::vector<Texture*> texture_;
	
#pragma region ConstBuff

	CBuff::CBuffColorMaterial* cMaterialMap_ = nullptr;
	ConstBuff material_;
	
#pragma endregion

	Vector4D color_ = { 1.0f,1.0f,1.0f,1.0f };
	std::string name_;
	int32_t width_ = 0;
	int32_t height_ = 0;
	Vector4D clearColor_ = { 0.1f,0.25f, 0.5f,0.0f };

	ComPtr<ID3D12DescriptorHeap> rtvHeap_;
	//	ビューポートシザー矩形
	ViewPortScissorRect viewPortSciRect_;

	DepthStencil dsv_;
public:
	void Initialize(int32_t width, int32_t height, const std::string& texName, int32_t textureNum = 1, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	void RSSetVPandSR();
	void SetGPipelineAndIAVertIdxBuff(GPipeline* pipeline);
	/**
	* @fn DrawLuminnce()
	* 描画処理関数
	*/
	void DrawLuminnce();
	/**
	* @fn Draw(GPipeline* , bool , int32_t)
	* 描画処理関数
	*/
	void Draw(GPipeline* pipeline, bool shadow, int32_t handle1 = -1);
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
	/**
	* @fn DrawIndexedInstanced()
	* 描画処理関数
	*/
	void Draw();

	void SetColor(const Vector4D& color);

#pragma region Getter

	int32_t GetWidth() const { return width_; }
	int32_t GetHeight() const { return height_; }
	const Vector4D& GetClearColor() { return clearColor_; }
	const std::string& GetName() { return name_; }

	ID3D12Resource* GetTextureBuff(int32_t index = 0) { return texture_[index]->GetResourceBuff(); }
	ID3D12Resource** GetTextureBuffPtr(int32_t index = 0) { return texture_[index]->GetResourceBuffAddress(); }
	int32_t GetTextureNum() { return texNum_; }
	ID3D12DescriptorHeap* GetRTVHeap() { return rtvHeap_.Get(); }
	ID3D12DescriptorHeap* GetDSVHeap() { return dsv_.GetDSVHeap(); }
	Texture* GetTexture() { return texture_[0]; }
	Texture* GetTexture(int32_t index) { return texture_[index]; }

#pragma endregion
};
