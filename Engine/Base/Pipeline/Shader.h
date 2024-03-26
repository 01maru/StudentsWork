#pragma once
#include <d3dcompiler.h>
#include <wrl.h>
#include <string>

/**
* @file Shader.h
* @brief シェーダー読み込み、コンパイル処理まとめたファイル
*/

namespace MNE
{

	class Shader
	{
	public:
		Shader() {};
		Shader(const std::string& VSFileName, const std::string& PSFileName, const LPCSTR& pEntryPoint = "main"
			, const std::string& GSFileName = "", const std::string& DSFileName = "", const std::string& HSFileName = "");
		/**
		* @fn Initialize(const LPCWSTR&, const LPCWSTR&, const LPCSTR&, const LPCWSTR&, const LPCWSTR&, const LPCWSTR&)
		* 初期化用関数
		* @param FileName それぞれファイルの名前(例 : L"shaderName")
		* @param pEntryPoint エントリーポイントの名前
		*/

		void Initialize(const std::string& VSFileName, const std::string& PSFileName, const LPCSTR& pEntryPoint = "main"
			, const std::string& GSFileName = "", const std::string& DSFileName = "", const std::string& HSFileName = "");

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
#pragma region Getter

		/**
		* @fn GetVSBlob()
		* ID3DBlob*型の頂点シェーダーオブジェクトを返す関数
		* @return ID3DBlob*型の頂点シェーダーオブジェクト
		*/
		ID3DBlob* GetVSBlob();

		/**
		* @fn GetHSBlob()
		* ID3DBlob*型のハルシェーダーオブジェクトを返す関数
		* @return ID3DBlob*型のハルシェーダーオブジェクト
		*/
		ID3DBlob* GetHSBlob();

		/**
		* @fn GetDSBlob()
		* ID3DBlob*型のドメインシェーダーオブジェクトを返す関数
		* @return ID3DBlob*型のドメインシェーダーオブジェクト
		*/
		ID3DBlob* GetDSBlob();

		/**
		* @fn GetGSBlob()
		* ID3DBlob*型のジオメトリシェーダーオブジェクトを返す関数
		* @return ID3DBlob*型のジオメトリシェーダーオブジェクト
		*/
		ID3DBlob* GetGSBlob();

		/**
		* @fn GetPSBlob()
		* ID3DBlob*型のピクセルシェーダーオブジェクトを返す関数
		* @return ID3DBlob*型のピクセルシェーダーオブジェクト
		*/
		ID3DBlob* GetPSBlob();

#pragma endregion
	};

}