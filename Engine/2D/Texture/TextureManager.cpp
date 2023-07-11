#include "TextureManager.h"
#include "DirectX.h"
#include <cassert>

#include "ImGuiManager.h"
#include "ImGuiController.h"
#include "ConvertString.h"

#include <DirectXTex.h>
using namespace DirectX;

Texture* TextureManager::sWhiteTexHandle = nullptr;

TextureManager* TextureManager::GetInstance()
{
	static TextureManager instance;
	return &instance;
}

void TextureManager::Initialize()
{
	ID3D12Device* device = MyDirectX::GetInstance()->GetDev();

#pragma region CmdList

	// コマンドアロケータを生成
	HRESULT result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&loadTexAllocator_));
	assert(SUCCEEDED(result));
	// コマンドリストを生成
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		loadTexAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&loadTexCmdList_));
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region CmdQueue

	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//コマンドキューを生成
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&loadTexQueue_));
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region Fence
	// フェンスの生成
	result = device->CreateFence(uploadTexFenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&uploadTexFence_));

#pragma endregion

	//	ロード失敗した際の白色テクスチャのロード
	sWhiteTexHandle = LoadTextureGraph("Resources/Sprite/white1x1.png");

	backSprite_ = std::make_unique<Sprite>();
	backSprite_->Initialize();
	backSprite_->SetColor(Vector4D(1.0f, 1.0f, 1.0f, 0.1f));

	previewSprite_ = std::make_unique<Sprite>();
	previewSprite_->Initialize();
}

void TextureManager::ImGuiTexUpdate()
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginChild();

	int32_t i = 0;
	for (auto itr = textures_.begin(); itr != textures_.end(); ++itr)
	{
		if (searchWord_.length() != 0) {
			//	Wordがなかったら
			if (itr->second->GetTextureName().find(searchWord_) == -1) continue;
		}

		imguiMan->PushID(i++);
		imguiMan->Text("Name : %s", itr->first.c_str());
		imguiMan->Text("Handle : %d", itr->second->GetHandle());
		if (imguiMan->SetRadioButton("PreviewTex", previewIdx_, i)) previewTexName_ = itr->first;

		imguiMan->Spacing();
		imguiMan->Separator();
		imguiMan->Spacing();

		imguiMan->PopID();
	}
	
	imguiMan->EndChild();
}

void TextureManager::ImGuiPreviewUpdate()
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->CheckBox("PreView", drawPreview_);
	imguiMan->Text("PreviewHandle : %d", previewIdx_);

	if (!drawPreview_) return;

	Vector2D pos = previewSprite_->GetPosition();
	imguiMan->SetSliderFloat2("Pos", pos);
	previewSprite_->SetPosition(pos);

	imguiMan->SetSliderFloat("Scale", previewSize_);
	Vector2D texsize = previewSprite_->GetTextureSize();
	previewSprite_->SetSize(texsize * previewSize_);
}

void TextureManager::ImGuiUpdate()
{
	if (!ImGuiController::GetInstance()->GetActiveTextureManager()) return;

	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("TextureManager");

	imguiMan->InputText("LoadPath", loadTexPath_);
	if (imguiMan->SetButton("Load")) {
		LoadTextureGraph(Util::ToWideString(loadTexPath_).c_str());

		UploadTexture();
	}
	imguiMan->InputText("Search", searchWord_);

	ImGuiPreviewUpdate();

	if (imguiMan->SetButton("Copy")) copyTexName_ = previewTexName_;

	int32_t prevIdx = previewIdx_;

	ImGuiTexUpdate();

	imguiMan->EndWindow();

	if (prevIdx != previewIdx_) {
		previewSprite_->SetHandle(textures_[previewTexName_].get());
		previewSize_ = 1.0f;
	}
	PreviewUpdate();
}

void TextureManager::PreviewUpdate()
{
	if (!drawPreview_) return;

	backSprite_->SetSize(previewSprite_->GetSize());
	backSprite_->SetPosition(previewSprite_->GetPosition());

	backSprite_->Update();
	previewSprite_->Update();
}

void TextureManager::DrawPreview()
{
	if (!drawPreview_)	return;

	backSprite_->Draw();
	previewSprite_->Draw();
}

Texture* TextureManager::LoadTextureGraph(const std::string& textureName)
{
	HRESULT result;
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	MyDirectX* dx = MyDirectX::GetInstance();

	//	既に画像読み込まれているかの確認
	if (textures_.count(textureName) != 0) {
		return textures_[textureName].get();
	}

	result = LoadFromWICFile(
		Util::ToWideString(textureName).c_str(),
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	if (!SUCCEEDED(result)) {
		//	読み込みに失敗したら白色画像
		return sWhiteTexHandle;
	}

	if (textures_.size() >= texExist_.size()) texExist_.emplace_back();
	int32_t index = 0;		//	画像のindex
	for (size_t i = 0; i < texExist_.size(); i++)
	{
		//	画像がその配列になかったら
		if (texExist_[i] == false)
		{
			texExist_[i] = true;
			index = (int32_t)i;
			break;
		}
	}

	std::unique_ptr<Texture> tex = std::make_unique<Texture>();
	textures_.emplace(textureName, std::move(tex));
	Texture* texture = textures_[textureName].get();

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
		IID_PPV_ARGS(texture->GetResourceBuffAddress()));

	//	FootPrint取得
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	UINT64 total_bytes = 0;
	dx->GetDev()->GetCopyableFootprints(&tectureResourceDesc, 0, 1, 0, &footprint, nullptr, nullptr, &total_bytes);

#pragma region Upload
	D3D12_RESOURCE_DESC uploadDesc{};

	textureUploadBuff_.emplace_back();

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
		IID_PPV_ARGS(textureUploadBuff_.back().ReleaseAndGetAddressOf()));
#pragma endregion

	//	転送
	uint8_t* mapforImg = nullptr;
	result = textureUploadBuff_.back().Get()->Map(0, nullptr, (void**)&mapforImg);	//	map

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
	textureUploadBuff_.back().Get()->Unmap(0, nullptr);	//	unmap

#pragma region CopyCommand
	//	グラフィックボード上のコピー先アドレス
	D3D12_TEXTURE_COPY_LOCATION texCopyDest{};
	texCopyDest.pResource = texture->GetResourceBuff();
	texCopyDest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	texCopyDest.SubresourceIndex = 0;
	//	グラフィックボード上のコピー元アドレス
	D3D12_TEXTURE_COPY_LOCATION src{};
	src.pResource = textureUploadBuff_.back().Get();
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint = footprint;

	//	作成
	loadTexCmdList_.Get()->CopyTextureRegion(&texCopyDest, 0, 0, 0, &src, nullptr);

	//	resourceBarrier挿入
	D3D12_RESOURCE_BARRIER copyBarrier{};
	copyBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	copyBarrier.Transition.pResource = texture->GetResourceBuff();
	copyBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	copyBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	copyBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	loadTexCmdList_.Get()->ResourceBarrier(1, &copyBarrier);

#pragma region SetSRV
	UINT incrementSize = dx->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = dx->GetCPUSRVHeapForHeapStart();
	srvHandle.ptr += incrementSize * index;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	dx->GetDev()->CreateShaderResourceView(texture->GetResourceBuff(), &srvDesc, srvHandle);
#pragma endregion

	texture->Initialize(textureName, index, texture->GetResourceBuff());
	return texture;
}

Texture* TextureManager::LoadTextureGraph(const wchar_t* textureName)
{
	return LoadTextureGraph(Util::ConvertToString(textureName));
}

Texture* TextureManager::CreateNoneGraphTexture(const std::string& texName)
{
	if (textures_.size() >= texExist_.size()) texExist_.emplace_back();
	int32_t index = 0;		//	画像のindex
	for (size_t i = 0; i < texExist_.size(); i++)
	{
		//	画像がその配列になかったら
		if (texExist_[i] == false)
		{
			texExist_[i] = true;
			index = (int32_t)i;
			break;
		}
	}

	if (textures_.count(texName) != 0) {
		assert(0);
	}

	std::unique_ptr<Texture> tex = std::make_unique<Texture>();
	textures_.emplace(texName, std::move(tex));

	Texture* texture = textures_[texName].get();

	texture->Initialize(texName, index, texture->GetResourceBuff());

	return texture;
}

void TextureManager::UploadTexture()
{
	if (textureUploadBuff_.empty()) return;

	// 命令のクローズ
#pragma region CmdClose

	HRESULT result = loadTexCmdList_->Close();
	assert(SUCCEEDED(result));
	// 溜めていたコマンドリストの実行(close必須)
	ID3D12CommandList* commandLists[] = { loadTexCmdList_.Get() };
	loadTexQueue_->ExecuteCommandLists(1, commandLists);

#pragma endregion CmdClose

#pragma region ChangeScreen

	// コマンドの実行完了を待つ
	loadTexQueue_->Signal(uploadTexFence_.Get(), ++uploadTexFenceVal_);
	if (uploadTexFence_->GetCompletedValue() != uploadTexFenceVal_)	//	GPUの処理が完了したか判定
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		uploadTexFence_->SetEventOnCompletion(uploadTexFenceVal_, event);
		if (event != 0) {
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
	// キューをクリア
	result = loadTexAllocator_->Reset();
	assert(SUCCEEDED(result));
	// 再びコマンドリストを貯める準備
	result = loadTexCmdList_->Reset(loadTexAllocator_.Get(), nullptr);
	assert(SUCCEEDED(result));

	textureUploadBuff_.clear();

#pragma endregion ChangeScreen
}

void TextureManager::DeleteTextureData(const std::string& textureName)
{
	//	データがあったら
	assert(textures_.count(textureName) != 0);

	int32_t handle = textures_[textureName]->GetHandle();
	//	範囲外参照回避用
	assert(!(texExist_.size() >= handle));

	if (texExist_[handle])	texExist_[handle] = false;

	textures_.erase(textureName);
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetTextureHandle(int32_t handle)
{
	MyDirectX* dx = MyDirectX::GetInstance();
	UINT incrementSize = dx->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = dx->GetGPUSRVHeapForHeapStart();
	srvGpuHandle.ptr += incrementSize * handle;
	return srvGpuHandle;
}
