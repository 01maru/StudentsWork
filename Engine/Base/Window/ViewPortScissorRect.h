#pragma once
#include <d3d12.h>
#include <vector>

/**
* @file ViewPortScissorRect.h
* @brief ビューポートとシザー矩形のファイル
*/

namespace MNE
{

	class ViewPortScissorRect
	{
	private:
		std::vector<D3D12_VIEWPORT> viewport_;
		std::vector<D3D12_RECT> scissorRect_;
	public:
		ViewPortScissorRect() {};

		/**
		* @fn InitializeVP(int, int, int, int, float, float, int)
		* ビューポート初期化用関数
		*/
		void InitializeVP(int width, int height, int topLftX, int topLftY, float MinDepth = 0.0f, float MaxDepth = 1.0f, int viewportNum = 1);
		/**
		* @fn InitializeSR(int, int, int, int, int)
		* シザー矩形初期化用関数
		*/
		void InitializeSR(int left, int width, int top, int height, int rectNum = 1);
		/**
		* @fn RSSetVPandSR()
		* コマンドリストにビューポートとシザー矩形をセットする
		*/
		void RSSetVPandSR();
	};

}
