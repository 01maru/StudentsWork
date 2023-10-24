#include "UIDrawer.h"
#include "UIEditor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "ConvertString.h"
#include "TextureManager.h"

void UIDrawer::Initialize()
{
	count_->StartCount();
}

void UIDrawer::Update(int16_t /*inputValue*/)
{
	//buttonMan_.Update(inputValue);
	//buttonMan_.MatUpdate();

	for (auto& sprite : obj_) {
		sprite.second->Update();
		sprite.second->MatUpdate();
	}

	//	アニメーション
		//	アニメーションタイマーがあるか検索
			//	あったら比較してカウントアクティブにする
	if (count_ != nullptr) {
		count_->Update();
	}
}

void UIDrawer::Draw()
{
	for (auto& sprite : obj_) {
		//if (sprite.second.GetTags() & data_->tagName_[activeTagName_]) {
			sprite.second->Draw();
		//}
	}

	//buttonMan_.Draw();
}

void UIDrawer::SetUI(const std::string& uiName)
{
	if (tagName_.count(uiName) == 0) return;

	activeTagName_ = uiName;

	//	ボタン初期化
	//buttonMan_.SetNumber(tagName_[activeTagName_]);
}

void UIDrawer::LoadSprites(const std::string& filename)
{
	LoadData(filename);
}
