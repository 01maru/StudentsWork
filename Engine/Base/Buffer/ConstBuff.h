#pragma once
#include <d3d12.h>
#include <cstdint>
#include <wrl.h>

/**
* @file ConstBuff.h
* @brief 定数バッファをまとめたファイル
*/

namespace MNE
{

	class ConstBuff
	{
	private:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	private:
		ComPtr<ID3D12Resource> material_;

	public:
		/**
		* @fn Initialize(uint64_t)
		* 初期化用関数
		*/
		void Initialize(uint64_t resWidth);
		/**
		* @fn SetGraphicsRootCBuffView(uint32_t)
		* コマンドリストのSetGraphicsRootCBuffViewを設定する関数
		* @param lootparaIdx ルートパラメータの添え字
		*/
		void SetGraphicsRootCBuffView(uint32_t rootparaIdx);

	#pragma region Getter

		ID3D12Resource* GetResource();

	#pragma endregion
	};

}
