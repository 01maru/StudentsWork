#include "UIManager.h"
#include "ImGuiManager.h"
#include "ImGuiController.h"
#include <iostream>

void UIManager::DeleteSpriteForList()
{
	for (auto itr = eraseSpriteName_.begin(); itr != eraseSpriteName_.end(); ++itr)
	{
		sprites_.erase(*itr);
	}
	eraseSpriteName_.clear();
}

void UIManager::AddSprite()
{
	Sprite sprite;
	sprite.Initialize(nullptr);
	sprites_.emplace(std::to_string(sprites_.size()), sprite);
}

void UIManager::ReNameSprite(std::map<std::string, Sprite, std::less<>>::iterator& itr)
{
	//	既にある名前だったら
	if (sprites_.count(spritename_) == 1) return;

	Sprite sprite = itr->second;
	eraseSpriteName_.push_back(itr->first);
	sprites_.emplace(spritename_, sprite);
}

void UIManager::DrawSpriteInfo(std::map<std::string, Sprite, std::less<>>::iterator& itr)
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	if (imguiMan->CollapsingHeader(itr->first))
	{
		Sprite* sprite = &itr->second;
		std::string name = itr->first;

		if (imguiMan->SetButton(name + " : ReName")) {
			ReNameSprite(itr);
			return;
		}

		Vector2D pos = sprite->GetPosition();
		imguiMan->SetSliderFloat2(name + " : PosLeftTop", pos);
		sprite->SetPosition(pos);

		Vector2D size = sprite->GetSize();
		imguiMan->SetSliderFloat2(name + " : Size", size);
		sprite->SetSize(size);

		imguiMan->Text("Texture");
		std::string texName = sprite->GetTexture()->GetTextureName();
		imguiMan->InputText(name + " : TexName ", texName, texName.length() + 1);

		pos = sprite->GetTextureLeftTop();
		imguiMan->SetSliderFloat2(name + " : TexLeftTop", pos);
		sprite->SetTextureLeftTop(pos);

		size = sprite->GetTextureSize();
		imguiMan->SetSliderFloat2(name + " : TexSize ", size);
		sprite->SetTextureSize(size);

		if (imguiMan->SetButton(name + " : Delete")) 	eraseSpriteName_.push_back(itr->first);
	}
}

UIManager* UIManager::GetInstance()
{
	static UIManager instance;
	return &instance;
}

void UIManager::Initialize()
{
	//sprites_.reserve(50);
}

void UIManager::ImGuiUpdate()
{
	if (!ImGuiController::GetInstance()->GetActiveUIManager()) return;

	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("UIManager", true);

	if (imguiMan->BeginMenuBar()) {
		if (imguiMan->BeginMenu("File")) {
			//if (imguiMan->MenuItem("Load")) ;
			//if (imguiMan->MenuItem("Save")) ;
			if (imguiMan->MenuItem("New")) editUI_ = true;
			imguiMan->EndMenu();
		}
		imguiMan->EndMenuBar();
	}

	imguiMan->InputText("FileName", filename_, 30);

	imguiMan->CheckBox("GlayScale", activeGlayscale_);

	if (editUI_) {
		imguiMan->Spacing();
		imguiMan->Separator();
		imguiMan->Spacing();

		imguiMan->InputText("SpriteName", spritename_, 500);
	
		if (imguiMan->SetButton("AddSprite"))	AddSprite();
	
		imguiMan->BeginChild();

		for (auto itr = sprites_.begin(); itr != sprites_.end(); ++itr)
		{
			DrawSpriteInfo(itr);

			imguiMan->Spacing();
		}

		DeleteSpriteForList();

		imguiMan->EndChild();
	}

	imguiMan->EndWindow();

	for (auto& sprite : sprites_) {
		sprite.second.Update();
	}
}

void UIManager::Draw()
{
	for (auto& sprite : sprites_) {
		sprite.second.Draw();
	}
}
