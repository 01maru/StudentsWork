#pragma once
#include "Sprite.h"

/**
* @file DebugTextManager.h
* @brief デバッグテキスト表示用ファイル
*/

namespace MNE
{

	class DebugTextManager
	{
	private:
		static Texture* sFontTex;		// デバッグテキスト用のテクスチャ
		static size_t sMaxSpritesNum;	// 最大文字数
		static size_t sFontWidth;		// fontTex内1文字分の横幅
		static size_t sFontHeight;		// fontTex内1文字分の縦幅
		static size_t sFontLineCharNum;	// fontTex内1行分の文字数

		std::vector<MNE::Sprite> sprites;

		size_t index = 0;

		DebugTextManager() {};
		~DebugTextManager() {};
	public:
		static DebugTextManager* GetInstance();
		DebugTextManager(const DebugTextManager& obj) = delete;
		DebugTextManager& operator=(const DebugTextManager& obj) = delete;

		/**
		* @fn Initialize()
		* 初期化用関数
		*/
		void Initialize();
		/**
		* @fn Print(const std::string&, const Vector2D&, float)
		* 正規化用関数
		* @param text 表示する文字列
		* @param pos 表示する文字列の左上座標
		* @param size 文字サイズ
		*/
		void Print(const std::string& text, const Vector2D& pos, float size);
		/**
		* @fn Draw()
		* 描画処理関数
		*/
		void Draw();
	};

}
