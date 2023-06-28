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

//	ぺらポリゴン
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

	int32_t width_ = 0;
	int32_t height_ = 0;
	Vector4D clearColor_ = { 0.1f,0.25f, 0.5f,0.0f };

	ComPtr<ID3D12DescriptorHeap> rtvHeap_;
	//	ビューポートシザー矩形
	ViewPortScissorRect viewPortSciRect_;

	DepthStencil dsv_;
public:
	void Initialize(int32_t width, int32_t height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	void Initialize(int32_t width, int32_t height, int32_t textureNum, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	void RSSetVPandSR();
	void DrawLuminnce();
	void SetGPipelineAndIAVertIdxBuff(GPipeline* pipeline);
	void Draw(GPipeline* pipeline, bool shadow, int32_t handle1 = -1);
	void DrawDoF();
	void DrawTask();
	void DrawMultiTask();
	void SetColor(const Vector4D& color);

	void Draw();

	//	Getter
	int32_t const GetWidth() { return width_; }
	int32_t const GetHeight() { return height_; }
	const Vector4D& GetClearColor() { return clearColor_; }

	ID3D12Resource* GetTextureBuff(int32_t index = 0) { return texture_[index]->GetResourceBuff(); }
	ID3D12Resource** GetTextureBuffPtr(int32_t index = 0) { return texture_[index]->GetResourceBuffAddress(); }
	int32_t GetTextureNum() { return texNum_; }
	ID3D12DescriptorHeap* GetRTVHeap() { return rtvHeap_.Get(); }
	ID3D12DescriptorHeap* GetDSVHeap() { return dsv_.GetDSVHeap(); }
	Texture* GetTexture() { return texture_[0]; }
	Texture* GetTexture(int32_t index) { return texture_[index]; }
};
