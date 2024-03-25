#include "Shader.h"
#include "ConvertString.h"
#include <string>
#include <cassert>

using namespace MNE::Util;

//-----------------------------------------------------------------------------
// [SECTION] Constructor
//-----------------------------------------------------------------------------

MNE::Shader::Shader(const std::string& VSFileName, const std::string& PSFileName, const LPCSTR& pEntryPoint, const std::string& GSFileName, const std::string& DSFileName, const std::string& HSFileName)
{
	Initialize(VSFileName, PSFileName, pEntryPoint, GSFileName, DSFileName, HSFileName);
}

//-----------------------------------------------------------------------------
// [SECTION] Initialize
//-----------------------------------------------------------------------------

void MNE::Shader::Initialize(const std::string& VSFileName, const std::string& PSFileName, const LPCSTR& pEntryPoint, const std::string& GSFileName, const std::string& DSFileName, const std::string& HSFileName)
{
	std::string filePath = "Resources/Shader/";
	std::wstring vs = ToWideString(filePath + VSFileName + ".hlsl");
	std::wstring ps = ToWideString(filePath + PSFileName + ".hlsl");

#pragma region VertexShader
	//	頂点シェーダファイル読み込み＆コンパイル
	HRESULT result = D3DCompileFromFile(
		vs.c_str(),											// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
		pEntryPoint, "vs_5_0",								// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// エラーなら
	Error(result);
#pragma endregion

#pragma region HS
	if (HSFileName.empty() == FALSE) {
		std::wstring hs = ToWideString(filePath + HSFileName + ".hlsl");

		//	頂点シェーダファイル読み込み＆コンパイル
		result = D3DCompileFromFile(
			hs.c_str(),									// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
			pEntryPoint, "hs_5_0",									// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
			0,
			&hsBlob, &errorBlob);

		// エラーなら
		Error(result);
	}
#pragma endregion

#pragma region DS
	if (DSFileName.empty() == FALSE) {
		std::wstring ds = ToWideString(filePath + DSFileName + ".hlsl");

		//	頂点シェーダファイル読み込み＆コンパイル
		result = D3DCompileFromFile(
			ds.c_str(),									// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
			pEntryPoint, "ds_5_0",									// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
			0,
			&dsBlob, &errorBlob);

		// エラーなら
		Error(result);
	}
#pragma endregion

#pragma region GS
	if (GSFileName.empty() == FALSE) {
		std::wstring gs = ToWideString(filePath + GSFileName + ".hlsl");

		//	頂点シェーダファイル読み込み＆コンパイル
		result = D3DCompileFromFile(
			gs.c_str(),									// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
			pEntryPoint, "gs_5_0",									// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
			0,
			&gsBlob, &errorBlob);

		// エラーなら
		Error(result);
	}
#pragma endregion

#pragma region PixelShader
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		ps.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		pEntryPoint, "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// エラーなら
	Error(result);
#pragma endregion
}

void MNE::Shader::Error(HRESULT result)
{
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

ID3DBlob* MNE::Shader::GetVSBlob()
{
	return vsBlob.Get();
}

ID3DBlob* MNE::Shader::GetHSBlob()
{
	return hsBlob.Get();
}

ID3DBlob* MNE::Shader::GetDSBlob()
{
	return dsBlob.Get();
}

ID3DBlob* MNE::Shader::GetGSBlob()
{
	return gsBlob.Get();
}

ID3DBlob* MNE::Shader::GetPSBlob()
{
	return psBlob.Get();
}
