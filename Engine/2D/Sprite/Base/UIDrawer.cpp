#include "UIDrawer.h"
#include "UIEditor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "ConvertString.h"
#include "TextureManager.h"

void UIDrawer::Update(int16_t inputValue)
{
	if (data_->buttonMan_ != nullptr) {
		data_->buttonMan_->Update(inputValue);
	}

	for (auto& sprite : data_->sprites_) {
		sprite.second.Update();
	}
}

void UIDrawer::Draw()
{
	for (auto& sprite : data_->sprites_) {
		if (sprite.second.GetTags() & data_->tagName_[activeTagName_]) {
			sprite.second.Draw();
		}
	}
}

void UIDrawer::SetUI(const std::string& uiName)
{
	if (data_->tagName_.count(uiName) == 0) return;

	activeTagName_ = uiName;
}

void UIDrawer::LoadSprites(const std::string& filename)
{
	data_ = UIEditor::GetInstance()->LoadUIData(filename);
}
