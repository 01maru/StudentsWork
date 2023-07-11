#include "OptionScene.h"

void OptionScene::Initialize(const std::string& filename)
{
	LoadSprites(filename);
}

void OptionScene::Update()
{
	if (!active_) return;

	UIData::Update();
}

void OptionScene::Draw()
{
	if (!active_) return;

	UIData::Draw();
}
