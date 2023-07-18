#pragma once
#include "Texture.h"
#include <vector>
#include <memory>
#include <map>
#include <list>
#include "Sprite.h"

class TextureManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Texture* sWhiteTexHandle;

	std::map<std::string, std::unique_ptr<Texture>, std::less<>> textures_;
	std::vector<bool> texExist_;

	ComPtr<ID3D12CommandAllocator> loadTexAllocator_;
	ComPtr<ID3D12GraphicsCommandList> loadTexCmdList_;

	ComPtr<ID3D12CommandQueue> loadTexQueue_;

	ComPtr<ID3D12Fence> uploadTexFence_;
	UINT64 uploadTexFenceVal_ = 0;
	std::list<ComPtr<ID3D12Resource>> textureUploadBuff_;

	bool drawPreview_ = false;
	float previewSize_ = 1.0f;
	std::unique_ptr<Sprite> previewSprite_;
	std::unique_ptr<Sprite> backSprite_;

	int32_t previewIdx_ = 0;
	std::string previewTexName_;
	std::string copyTexName_;

	std::string searchWord_;
	std::string loadTexPath_;

private:
	TextureManager() {}
	~TextureManager() {}

	void ImGuiPreviewUpdate();
	void ImGuiTexUpdate();
	void PreviewUpdate();

public:
	static TextureManager* GetInstance();
	TextureManager(const TextureManager& obj) = delete;
	TextureManager& operator=(const TextureManager& obj) = delete;

	void Initialize();
	void ImGuiUpdate();
	void DrawPreview();

	Texture* LoadTextureGraph(const std::string& textureName, const std::string& path = "Resources/Sprite/");
	Texture* CreateNoneGraphTexture(const std::string& texName);
	void UploadTexture();

	//	Delete
	void DeleteTextureData(const std::string& textureName);

	//	Getter
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandle(int32_t handle);
	static Texture* GetWhiteTexture() { return sWhiteTexHandle; }
	Texture* PasteTexture() { return textures_[copyTexName_].get(); }
};

