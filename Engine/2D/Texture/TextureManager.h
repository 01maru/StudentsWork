﻿#pragma once
#include "Texture.h"
#include <vector>

class TextureManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Texture sWhiteTexHandle;

	int32_t textureNum_ = 0;
	std::vector<ComPtr<ID3D12Resource>> texBuff_;
	std::vector<ComPtr<ID3D12Resource>> uploadBuff_;
	std::vector<bool> texExist_;

	TextureManager() {}
	~TextureManager() {}
public:
	void SetWhiteTexHandle();		//	読み込み失敗用白色画像のセット
	static TextureManager* GetInstance();
	TextureManager(const TextureManager& obj) = delete;
	TextureManager& operator=(const TextureManager& obj) = delete;

	void Initialize();
	void DeleteTexture(int32_t handle);
	Texture LoadTextureGraph(const wchar_t* textureName);
	void CreateNoneGraphTexture(const std::string& texName, Texture& texture);
	//	Getter
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandle(int32_t handle);
	const Texture& GetWhiteTexture() { return sWhiteTexHandle; }
};

