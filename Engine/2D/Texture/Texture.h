#pragma once
#include "Vector2D.h"
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <cstdint>

/**
* @file Texture.h
* @brief テクスチャファイル
*/

namespace MNE
{

	class Texture
	{
	private:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		std::string name_;
		//	初期値は-1
		int32_t handle_ = -1;
		ComPtr<ID3D12Resource> texBuff_;
	public:
		/**
		* @fn Initialize(const std::string&, int32_t, ID3D12Resource*)
		* 初期化用関数
		*/
		void Initialize(const std::string& texName, int32_t handle, ID3D12Resource* texBuff_ptr);

	#pragma region Getter

		/**
		* @fn GetTextureName()
		* name_のGetter関数
		* @return name_の値
		*/
		const std::string& GetTextureName() { return name_; }
		/**
		* @fn GetHandle()
		* handle_のGetter関数
		* @return handle_の値
		*/
		int32_t GetHandle() { return handle_; }
		/**
		* @fn GetResourceBuff()
		* texBuff_.Get()のGetter関数
		* @return texBuff_.Get()の値
		*/
		ID3D12Resource* GetResourceBuff() { return texBuff_.Get(); }
		/**
		* @fn GetResourceBuffAddress()
		* texBuff_.ReleaseAndGetAddressOf()のGetter関数
		* @return texBuff_.ReleaseAndGetAddressOf()の値
		*/
		ID3D12Resource** GetResourceBuffAddress() { return texBuff_.ReleaseAndGetAddressOf(); }

		MyMath::Vector2D GetTextureSize();

	#pragma endregion
	};

}
