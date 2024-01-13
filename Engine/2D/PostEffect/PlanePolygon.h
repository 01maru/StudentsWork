#pragma once
#include "VertIdxBuff.h"

/**
* @file PlanePolygon.h
* @brief 板ポリを表示する用ファイル
*/

namespace MNE
{

	class PlanePolygon :public VertIdxBuff
	{
	private:
		std::vector<MNE::ScreenVertex> vertices_;
		std::vector<uint16_t> indices_;

	private:
		void SetVertices() override;

	public:
		/**
		* @fn Initialize()
		* 初期化用関数
		*/
		void Initialize();
		/**
		* @fn DrawIndexedInstanced()
		* 描画処理関数
		*/
		void DrawIndexedInstanced();
	};

}
