#include "UIData.h"
#include "UIEditor.h"

void UIData::Update()
{
	for (auto& sprite : sprites_) {
		sprite.second.Update();
	}
}

void UIData::Draw()
{
	for (auto& sprite : sprites_) {
		if (sprite.second.GetTags() & drawTag_) {
			sprite.second.Draw();
		}
	}
}

void UIData::LoadSprites(const std::string& filename)
{
	sprites_ = UIEditor::GetInstance()->LoadFile(filename);
}
