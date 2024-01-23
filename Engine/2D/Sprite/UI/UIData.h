#pragma once
#include "UIButtonManager.h"
#include "FrameCounter.h"
#include "UIObject.h"
#include <map>
#include <unordered_map>

/**
* @file UIData.h
* @brief UIEditor用のUIData構造体ファイル
*/

namespace MNE
{

	class UIData
	{
	public:
		/**
		* @fn Finalize()
		* 終了処理関数(Editor用)
		*/
		void Finalize();
		/**
		* @fn InputUpdate()
		* 入力更新処理関数
		*/
		void InputUpdate(int16_t inputValue);
		/**
		* @fn Update()
		* 更新処理関数
		*/
		void Update();
		/**
		* @fn Draw()
		* 描画処理関数
		*/
		void Draw();

	protected:
		//	開始時アニメーションか終了時アニメーションか
		bool startAnimation_ = true;
		//	全体のFrameCounter
		std::unique_ptr<FrameCounter> count_;
		//	選択中のボタン管理用変数
		std::unique_ptr<UIButtonManager> buttonMan_;
		//	表示するUIObjectのマップ変数
		std::unordered_map<std::string, std::unique_ptr<UIObject>> obj_;

		//	現在使用してないタグ
		std::map<std::string, uint16_t, std::less<>> tagName_;

		std::string activeTag_ = "";

		const uint16_t ACTIVE_ALL_TAG = 0b1111111111111111;

	public:
		/**
		* @fn LoadData(const std::string&)
		* レベルデータ読み込み用関数
		* @param filename レベルデータのファイル名
		*/
		void LoadData(const std::string& filename);

	#pragma region Getter

		/**
		* @fn GetSelectName()
		* 選択中のボタンの名前のGetter関数
		* @return 選択中のボタンの名前
		*/
		const std::string& GetSelectName();
		/**
		* @fn GetSelectPosition()
		* 選択中の座標のGetter関数(カーソル用)
		* @return 選択中の座標
		*/
		Vector2D& GetSelectPosition();
		/**
		* @fn GetSelectSize()
		* 選択中のボタンサイズのGetter関数(カーソルアニメーション用)
		* @return 選択中のサイズ
		*/
		Vector2D& GetSelectSize();

		UIObject* GetUIObject(const std::string& name);

		bool GetIsEndAnimation();

	#pragma endregion

		void Reset();

		void ResetAnimation(bool startingAnimation);

		uint16_t GetTag() { 
			if (tagName_.count(activeTag_) == 0)
			{
				return ACTIVE_ALL_TAG;
			}
			return tagName_[activeTag_]; }

	#pragma region Setter

		void SetSelectButton(const std::string& name);

	#pragma endregion
	};

}
