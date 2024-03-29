#include "DirectX.h"
#include "Window.h"
#include "PostEffect.h"
#include <cassert>

using namespace MyMath;

// 対応レベルの配列
D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
};

MNE::MyDirectX* MNE::MyDirectX::GetInstance()
{
	static MyDirectX instance;
	return &instance;
}

void MNE::MyDirectX::DebugLayer()
{
	//	オン
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
}

void MNE::MyDirectX::Initialize()
{
#ifdef _DEBUG
	DebugLayer();
#endif

#pragma region GPU列挙
#pragma region Adapter
	ComPtr<IDXGIFactory7> dxgiFactory;
	// DXGIファクトリーの生成
	HRESULT result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));
	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter;
	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (size_t i = 0;
		dxgiFactory->EnumAdapterByGpuPreference((UINT)i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		// 動的配列に追加する
		adapters.push_back(tmpAdapter);
	}

	//	グラボアダプタ選別
	// 妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);
		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i].Get();
			break;
		}
	}
#pragma endregion Adapter

#pragma region Device
	//	デバイス生成
	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < _countof(levels); i++) {
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(&device_));
		if (result == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
#pragma endregion Device
#pragma endregion

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	device_.As(&infoQueue);

	D3D12_MESSAGE_ID denyIds[] = {
	  D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
	  D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
	};
	D3D12_MESSAGE_SEVERITY severities[] = {
	  D3D12_MESSAGE_SEVERITY_INFO
	};
	D3D12_INFO_QUEUE_FILTER filter{};
	filter.DenyList.NumIDs = _countof(denyIds);
	filter.DenyList.pIDList = denyIds;
	filter.DenyList.NumSeverities = _countof(severities);
	filter.DenyList.pSeverityList = severities;

	infoQueue->PushStorageFilter(&filter);

	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
	}
#endif

#pragma region CmdList
	// コマンドアロケータを生成
	result = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator_));
	assert(SUCCEEDED(result));
	// コマンドリストを生成
	result = device_->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&cmdList_));
	assert(SUCCEEDED(result));
#pragma endregion CmdList

#pragma region CmdQueue
	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//コマンドキューを生成
	result = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&cmdQueue_));
	assert(SUCCEEDED(result));
#pragma endregion CmdQueue

#pragma region DoubleBuffering
#pragma region swapChain
	ComPtr<IDXGISwapChain1> swapChain1;
	// 設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = Window::sRESOLUTION_WID;					//	画面幅解像度
	swapChainDesc.Height = Window::sRESOLUTION_HEIGHT;				//	画面高さ解像度
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// 色情報の書式
	swapChainDesc.SampleDesc.Count = 1;								// マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;				// バックバッファ用
	swapChainDesc.BufferCount = 2;									// バッファ数を2つに設定(表と裏)
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// フリップ後は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	// スワップチェーンの生成
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue_.Get(), Window::GetInstance()->GetHwnd(), &swapChainDesc, nullptr, nullptr,
		&swapChain1);

	swapChain1.As(&swapChain_);		//	1→4に変換
	assert(SUCCEEDED(result));
#pragma endregion swapChain

#pragma region DesHeap
	// デスクリプタヒープの設定(GPUにメモリ領域確保しそれから使う)
	rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	rtvHeapDesc_.NumDescriptors = swapChainDesc.BufferCount; // 裏表の2つ
	// デスクリプタヒープの生成
	device_->CreateDescriptorHeap(&rtvHeapDesc_, IID_PPV_ARGS(&rtvHeap_));
#pragma endregion DesHeap

#pragma region BackBuff
	backBuffers_.resize(swapChainDesc.BufferCount);
#pragma endregion BackBuff

#pragma region RTV
	// スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers_.size(); i++) {
		// スワップチェーンからバッファを取得
		swapChain_->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers_[i]));
		// デスクリプタヒープのハンドルを取得(先頭アドレス)
		rtvHandle_ = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かでアドレスがずれる
		rtvHandle_.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
		// レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		device_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc, rtvHandle_);
	}
#pragma endregion RTV
#pragma endregion

#pragma region SRV
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = rtvHeap_->GetDesc();
	const size_t kMaxSRVCount = 2056;
	//	heapSETTING
	heapDesc.NumDescriptors = kMaxSRVCount;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	
	result = device_->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(srvHeap_.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region 深度バッファ
	dsv_.Initialize(Window::sRESOLUTION_WID, Window::sRESOLUTION_HEIGHT, DXGI_FORMAT_D32_FLOAT);
#pragma endregion

#pragma region fence
	// フェンスの生成
	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
#pragma endregion fence

	//	ビューポート
	viewPortSciRect_.InitializeVP(Window::sRESOLUTION_WID, Window::sRESOLUTION_HEIGHT, 0, 0, 0.0f, 1.0f);
	// シザー矩形
	viewPortSciRect_.InitializeSR(0, Window::sRESOLUTION_WID, 0, Window::sRESOLUTION_HEIGHT);
}

void MNE::MyDirectX::SetResourceBarrier(D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, ID3D12Resource* pResource)
{
	if (pResource != nullptr) {
		barrierDesc_.Transition.pResource = pResource;
	}
	barrierDesc_.Transition.StateBefore = StateBefore;
	barrierDesc_.Transition.StateAfter = StateAfter;
	cmdList_->ResourceBarrier(1, &barrierDesc_);
}

void MNE::MyDirectX::ScreenClear(const MyMath::Vector4D& clearColor, D3D12_CPU_DESCRIPTOR_HANDLE& rtvHandle)
{
	FLOAT color[] = { clearColor.x,clearColor.y, clearColor.z,clearColor.w };
	cmdList_->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
}

void MNE::MyDirectX::CmdListDrawAble(ID3D12Resource* pResource, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, D3D12_CPU_DESCRIPTOR_HANDLE& rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE& dsvHandle, int32_t rtDescNum, const MyMath::Vector4D& clearColor)
{
	// 1.リソースバリアで書き込み可能に変更
#pragma region ReleaseBarrier
	SetResourceBarrier(StateBefore, StateAfter, pResource);
#pragma endregion ReleaseBarrier
	// 2.描画先の変更
#pragma region Change
	cmdList_->OMSetRenderTargets(static_cast<UINT>(rtDescNum), &rtvHandle, false, &dsvHandle);
#pragma endregion Change
	// 3.画面クリア
#pragma region ScreenClear
	ScreenClear(clearColor, rtvHandle);

	cmdList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion
}

void MNE::MyDirectX::PrevPostEffect(MNE::PostEffect* postEffect, const MyMath::Vector4D& clearColor)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = postEffect->GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_ = postEffect->GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();

	size_t num = postEffect->GetTextureNum();
	for (size_t i = 0; i < num; i++)
	{
		// 1.リソースバリアで書き込み可能に変更
#pragma region ReleaseBarrier
		SetResourceBarrier(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET, postEffect->GetTextureBuff((int32_t)i));
#pragma endregion ReleaseBarrier
	}

	// 2.描画先の変更
#pragma region Change
	cmdList_->OMSetRenderTargets((UINT)num, &rtvHandle, true, &dsvHandle_);
#pragma endregion Change

	// 3.画面クリア
#pragma region ScreenClear
	for (size_t i = 0; i < postEffect->GetTextureNum(); i++)
	{
		rtvHandle.ptr += device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * i;

		ScreenClear(clearColor, rtvHandle);
	}
	cmdList_->ClearDepthStencilView(dsvHandle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion

	postEffect->RSSetVPandSR();

	cmdList_->SetDescriptorHeaps(1, srvHeap_.GetAddressOf());
}

void MNE::MyDirectX::PostEffectDraw(MNE::PostEffect* postEffect)
{
	for (size_t i = 0; i < postEffect->GetTextureNum(); i++)
	{
		SetResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET
			, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, postEffect->GetTextureBuff((int32_t)i));
	}
}

void MNE::MyDirectX::PrevDraw(const MyMath::Vector4D& clearColor)
{
	// 1.リソースバリアで書き込み可能に変更
#pragma region ReleaseBarrier
	// バックバッファの番号を取得(2つなので0番か1番)
	UINT64 bbIndex = swapChain_->GetCurrentBackBufferIndex();		//	現在のバックバッファ設定
#pragma endregion ReleaseBarrier

	// 2.描画先の変更
#pragma region Change
	// レンダーターゲットビューのハンドルを取得
	rtvHandle_ = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	rtvHandle_.ptr += bbIndex * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_ = dsv_.GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();
#pragma endregion Change

	CmdListDrawAble(backBuffers_[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET, rtvHandle_, dsvHandle_, 1, clearColor);

	viewPortSciRect_.RSSetVPandSR();
}

void MNE::MyDirectX::PostDraw()
{
	// 5.リソースバリアを戻す
#pragma region ReleaseBarrier
	SetResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
#pragma endregion ReleaseBarrier
}

void MNE::MyDirectX::CmdListCloseAndFlip()
{
	// 命令のクローズ
#pragma region CmdClose
	HRESULT result = cmdList_->Close();
	assert(SUCCEEDED(result));
	// 溜めていたコマンドリストの実行(close必須)
	ID3D12CommandList* commandLists[] = { cmdList_.Get() };
	cmdQueue_->ExecuteCommandLists(1, commandLists);
	// 画面に表示するバッファをフリップ(裏表の入替え)
	result = swapChain_->Present(1, 0);
	assert(SUCCEEDED(result));
#pragma endregion CmdClose
}

void MNE::MyDirectX::DrawEnd()
{
	CmdListCloseAndFlip();

#pragma region ChangeScreen
	// コマンドの実行完了を待つ
	cmdQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_)	//	GPUの処理が完了したか判定
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		if (event != 0) {
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
	// キューをクリア
	HRESULT result = cmdAllocator_->Reset();
	assert(SUCCEEDED(result));
	// 再びコマンドリストを貯める準備
	result = cmdList_->Reset(cmdAllocator_.Get(), nullptr);
	assert(SUCCEEDED(result));
#pragma endregion ChangeScreen
}
