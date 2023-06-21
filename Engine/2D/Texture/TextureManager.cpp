﻿#include "TextureManager.h"
#include "DirectX.h"
#include <cassert>

#include "ImGuiManager.h"
#include "ImGuiController.h"

#include <DirectXTex.h>
using namespace DirectX;

Texture* TextureManager::sWhiteTexHandle = nullptr;

#pragma region string変換
std::string ConvertToString(const wchar_t* name)
{
	std::wstring ws = name;

	size_t inLen = (size_t)ws.length();
	size_t outLen = WideCharToMultiByte(CP_ACP, 0, ws.c_str(), (int)inLen, 0, 0, 0, 0);

	std::string result(outLen, '\0');
	if (outLen) WideCharToMultiByte(CP_ACP, 0, ws.c_str(), (int)inLen, &result[0], (int)outLen, 0, 0);

	return result;
}
#pragma endregion

TextureManager* TextureManager::GetInstance()
{
	static TextureManager instance;
	return &instance;
}

void TextureManager::Initialize()
{
	//textureNum_ = 0;

	//texExist_.clear();

	//	ロード失敗した際の白色テクスチャのロード
	sWhiteTexHandle = LoadTextureGraph(L"Resources/Sprite/white1x1.png");
}

void TextureManager::ImGuiUpdate()
{
	if (!ImGuiController::GetInstance()->GetActiveTextureManager()) return;

	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("VolumeManager", true);

	//if (imguiMan->BeginMenuBar()) {
	//	if (imguiMan->BeginMenu("File")) {
	//		if (imguiMan->MenuItem("Save")) ;
	//		if (imguiMan->MenuItem("Load")) ;
	//		imguiMan->EndMenu();
	//	}
	//	imguiMan->EndMenuBar();
	//}

	imguiMan->BeginChild(Vector2D(0, 200));

	for (size_t i = 0; i < textures_.size(); i++)
	{
		imguiMan->Text("Name : %s", textures_[i]->GetTextureName().c_str());
		imguiMan->Text("Handle : %d", textures_[i]->GetHandle());
	}
	imguiMan->EndChild();

	imguiMan->EndWindow();
}

Texture* TextureManager::LoadTextureGraph(const wchar_t* textureName)
{
	HRESULT result;
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	MyDirectX* dx = MyDirectX::GetInstance();

	//	既に画像読み込まれているかの確認
	for (int i = 0; i < textures_.size(); i++) {
		std::string textureName_str = ConvertToString(textureName);
		if (textures_[i]->GetTextureName() == textureName_str)
		{
			//	既にあったら
			return textures_[i].get();
		}
	}

	result = LoadFromWICFile(
		textureName,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	if (!SUCCEEDED(result)) {
		//	読み込みに失敗したら白色画像
		return sWhiteTexHandle;
	}

	//int32_t index = 0;		//	画像のindex
	//for (size_t i = 0; i < texExist_.size(); i++)
	//{
	//	//	画像がその配列になかったら
	//	if (texExist_[i] == false)
	//	{
	//		texExist_[i] = true;
	//		index = (int32_t)i;
	//		break;
	//	}
	//}
	//textureNum_++;

	//	indexを進める
	texIndex_++;
	textures_.emplace_back();
	textures_[texIndex_] = std::make_unique<Texture>();

	//	ミニマップ生成
	ScratchImage mipChain{};
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	const Image* img = scratchImg.GetImage(0, 0, 0);			//	生データ

	metadata.format = MakeSRGB(metadata.format);

	D3D12_RESOURCE_DESC tectureResourceDesc{};
	tectureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	tectureResourceDesc.Format = metadata.format;
	tectureResourceDesc.Width = metadata.width;
	tectureResourceDesc.Height = (UINT)metadata.height;
	tectureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	tectureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	tectureResourceDesc.SampleDesc.Count = 1;

	//	ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//int32_t buffIndex = index;
	//	テクスチャバッファ生成
	result = dx->GetDev()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&tectureResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(textures_[texIndex_]->GetResourceBuffAddress()));

	//	FootPrint取得
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	UINT64 total_bytes = 0;
	dx->GetDev()->GetCopyableFootprints(&tectureResourceDesc, 0, 1, 0, &footprint, nullptr, nullptr, &total_bytes);

#pragma region Upload
	D3D12_RESOURCE_DESC uploadDesc{};

	dx->CreateUploadBuffEmplaceBack();

	uploadDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//uploadDesc.Width = MyMath::AlignmentSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) * img->height;
	uploadDesc.Width = total_bytes;
	uploadDesc.Height = 1;
	uploadDesc.DepthOrArraySize = 1;
	uploadDesc.MipLevels = 1;
	uploadDesc.SampleDesc.Count = 1;
	uploadDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_HEAP_PROPERTIES uploadHeap{};
	uploadHeap.Type = D3D12_HEAP_TYPE_UPLOAD;

	dx->GetDev()->CreateCommittedResource(
		&uploadHeap,
		D3D12_HEAP_FLAG_NONE,
		&uploadDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,		//	CPUから書き込み可能、GPUは読み取り専用
		nullptr,
		IID_PPV_ARGS(dx->GetUploadResourceBuffAddress()));
#pragma endregion

	//	転送
	uint8_t* mapforImg = nullptr;
	result = dx->GetUploadResourceBuff()->Map(0, nullptr, (void**)&mapforImg);	//	map

	uint8_t* uploadStart = mapforImg + footprint.Offset;
	uint8_t* sourceStart = img->pixels;
	uint32_t sourcePitch = ((uint32_t)img->width * sizeof(uint32_t));

	//	画像の高さ(ピクセル)分コピーする
	for (uint32_t i = 0; i < footprint.Footprint.Height; i++)
	{
		memcpy(
			uploadStart + i * footprint.Footprint.RowPitch,
			sourceStart + i * sourcePitch,
			sourcePitch
		);
	}
	dx->GetUploadResourceBuff()->Unmap(0, nullptr);	//	unmap

#pragma region CopyCommand
	//	グラフィックボード上のコピー先アドレス
	D3D12_TEXTURE_COPY_LOCATION texCopyDest{};
	texCopyDest.pResource = textures_[texIndex_]->GetResourceBuff();
	texCopyDest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	texCopyDest.SubresourceIndex = 0;
	//	グラフィックボード上のコピー元アドレス
	D3D12_TEXTURE_COPY_LOCATION src{};
	src.pResource = dx->GetUploadResourceBuff();
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint = footprint;

	//	作成
	dx->GetLoadTexCmdList()->CopyTextureRegion(&texCopyDest, 0, 0, 0, &src, nullptr);

	//	resourceBarrier挿入
	D3D12_RESOURCE_BARRIER copyBarrier{};
	copyBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	copyBarrier.Transition.pResource = textures_[texIndex_]->GetResourceBuff();
	copyBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	copyBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	copyBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	dx->GetLoadTexCmdList()->ResourceBarrier(1, &copyBarrier);

#pragma region SetSRV
	UINT incrementSize = dx->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = dx->GetCPUSRVHeapForHeapStart();
	srvHandle.ptr += incrementSize * texIndex_;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	dx->GetDev()->CreateShaderResourceView(textures_[texIndex_]->GetResourceBuff(), &srvDesc, srvHandle);
#pragma endregion
	
	textures_[texIndex_]->Initialize(ConvertToString(textureName), texIndex_, textures_[texIndex_]->GetResourceBuff());
	return textures_[texIndex_].get();
}

Texture* TextureManager::CreateNoneGraphTexture(const std::string& texName)
{
	//int32_t index = 0;		//	画像のindex
	//for (size_t i = 0; i < texExist_.size(); i++)
	//{
	//	//	画像がその配列になかったら
	//	if (texExist_[i] == false)
	//	{
	//		texExist_[i] = true;
	//		index = (int32_t)i;
	//		break;
	//	}
	//}
	//textureNum_++;
	//int32_t buffIndex = index - 1;

	//texture.Initialize(texName, index, texBuff_[buffIndex].Get());
	texIndex_++;
	textures_.emplace_back();

	textures_[texIndex_] = std::make_unique<Texture>();
	textures_[texIndex_]->Initialize(texName, texIndex_, textures_[texIndex_]->GetResourceBuff());

	return textures_[texIndex_].get();
}

//void TextureManager::DeleteTexture(int32_t handle)
//{
//	if (texExist_[handle])
//	{
//		texExist_[handle] = false;
//	}
//}

//void TextureManager::DeleteTextureData(const std::string& textureName)
//{
//	//	データがあったら
//	assert(textures.count(textureName) != 0);
//
//	int32_t handle = textures[textureName]->GetHandle();
//	//	範囲外参照回避用
//	assert(!(texExist_.size() >= handle));
//
//	if (texExist_[handle])
//	{
//		texExist_[handle] = false;
//	}
//
//	textures.erase(textureName);
//}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetTextureHandle(int32_t handle)
{
	MyDirectX* dx = MyDirectX::GetInstance();
	UINT incrementSize = dx->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = dx->GetGPUSRVHeapForHeapStart();
	srvGpuHandle.ptr += incrementSize * handle;
	return srvGpuHandle;
}