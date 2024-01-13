#pragma once
#include <d3dcompiler.h>
#include <wrl.h>

/**
* @file Shader.h
* @brief シェーダー読み込み、コンパイル処理まとめたファイル
*/

namespace MNE
{

	class Shader
	{
	private:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		ComPtr<ID3DBlob> vsBlob;		// 頂点シェーダオブジェクト
		ComPtr<ID3DBlob> hsBlob;		// ハルシェーダオブジェクト
		ComPtr<ID3DBlob> dsBlob;		// ドメインシェーダオブジェクト
		ComPtr<ID3DBlob> gsBlob;		// ジオメトリシェーダオブジェクト
		ComPtr<ID3DBlob> psBlob;		// ピクセルシェーダオブジェクト
		ComPtr<ID3DBlob> errorBlob;		// エラーオブジェクト

	private:
		void Error(HRESULT result);

	public:
		Shader() {};
		Shader(const LPCWSTR& VSFileName, const LPCWSTR& PSFileName, const LPCSTR& pEntryPoint = "main"
			, const LPCWSTR& GSFileName = nullptr, const LPCWSTR& DSFileName = nullptr, const LPCWSTR& HSFileName = nullptr);
		/**
		* @fn Initialize(const LPCWSTR&, const LPCWSTR&, const LPCSTR&, const LPCWSTR&, const LPCWSTR&, const LPCWSTR&)
		* 初期化用関数
		* @param FileName パスを含めたそれぞれファイルの名前(例 : L"Resources/Shader/shaderName.hlsl")
		* @param pEntryPoint エントリーポイントの名前
		*/
		void Initialize(const LPCWSTR& VSFileName, const LPCWSTR& PSFileName, const LPCSTR& pEntryPoint = "main"
			, const LPCWSTR& GSFileName = nullptr, const LPCWSTR& DSFileName = nullptr, const LPCWSTR& HSFileName = nullptr);

#pragma region Getter

		/**
		* @fn GetVSBlob()
		* ID3DBlob*型の頂点シェーダーオブジェクトを返す関数
		* @return ID3DBlob*型の頂点シェーダーオブジェクト
		*/
		ID3DBlob* GetVSBlob() { return vsBlob.Get(); }

		/**
		* @fn GetHSBlob()
		* ID3DBlob*型のハルシェーダーオブジェクトを返す関数
		* @return ID3DBlob*型のハルシェーダーオブジェクト
		*/
		ID3DBlob* GetHSBlob() { return hsBlob.Get(); }

		/**
		* @fn GetDSBlob()
		* ID3DBlob*型のドメインシェーダーオブジェクトを返す関数
		* @return ID3DBlob*型のドメインシェーダーオブジェクト
		*/
		ID3DBlob* GetDSBlob() { return dsBlob.Get(); }

		/**
		* @fn GetGSBlob()
		* ID3DBlob*型のジオメトリシェーダーオブジェクトを返す関数
		* @return ID3DBlob*型のジオメトリシェーダーオブジェクト
		*/
		ID3DBlob* GetGSBlob() { return gsBlob.Get(); }

		/**
		* @fn GetPSBlob()
		* ID3DBlob*型のピクセルシェーダーオブジェクトを返す関数
		* @return ID3DBlob*型のピクセルシェーダーオブジェクト
		*/
		ID3DBlob* GetPSBlob() { return psBlob.Get(); }

#pragma endregion
	};

}