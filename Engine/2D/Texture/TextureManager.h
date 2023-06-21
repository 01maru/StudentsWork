#pragma once
#include "Texture.h"
#include <vector>
#include <memory>
#include <map>
#include <list>

class TextureManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Texture* sWhiteTexHandle;

	//	texturesのindex
	int texIndex_ = -1;
	//int32_t textureNum_ = 0;
	std::vector<std::unique_ptr<Texture>> textures_;

	//std::map<std::string, std::unique_ptr<Texture>, std::less<>> textures;
	//std::vector<bool> texExist_;

	ComPtr<ID3D12CommandAllocator> loadTexAllocator_;
	ComPtr<ID3D12GraphicsCommandList> loadTexCmdList_;

	ComPtr<ID3D12CommandQueue> loadTexQueue_;

	ComPtr<ID3D12Fence> uploadTexFence_;
	UINT64 uploadTexFenceVal_ = 0;
	std::list<ComPtr<ID3D12Resource>> textureUploadBuff_;

private:
	TextureManager() {}
	~TextureManager() {}

public:
	static TextureManager* GetInstance();
	TextureManager(const TextureManager& obj) = delete;
	TextureManager& operator=(const TextureManager& obj) = delete;

	void Initialize();
	void ImGuiUpdate();
	void UploadTexture();

	Texture* LoadTextureGraph(const wchar_t* textureName);
	Texture* CreateNoneGraphTexture(const std::string& texName);

	//Texture* CreateNoneGraphTexture(const std::string& textureName);

	//	Delete
	void DeleteTextureData(const std::string& textureName);

	//	Getter
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandle(int32_t handle);
	static Texture* GetWhiteTexture() { return sWhiteTexHandle; }
};

