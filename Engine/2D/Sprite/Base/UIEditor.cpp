﻿#include "UIEditor.h"
#include "ImGuiManager.h"
#include "ImGuiController.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include "TextureManager.h"
#include "Window.h"

std::map<std::string, Sprite, std::less<>> UIEditor::LoadFile(const std::string& filename)
{
	std::string filePath = "Resources/Levels/" + filename + ".txt";

	//ファイル開く(開けなかったら新規作成)
	std::ifstream file;
	file.open(filePath.c_str());

	std::map<std::string, Sprite, std::less<>> ans;

	if (file.fail()) {
		if (editUI_) return ans;
	}

	// 1行ずつ読み込む
	std::string line;
	while (getline(file, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		if (key.size() != 0) {
			std::string texname;
			line_stream >> texname;

			Sprite sprite;
			sprite.Initialize(TextureManager::GetInstance()->LoadTextureGraph(texname));

			Vector2D pos;
			Vector2D size;
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> size.x;
			line_stream >> size.y;

			sprite.SetPosition(pos);
			sprite.SetSize(size);

			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> size.x;
			line_stream >> size.y;

			sprite.SetTextureLeftTop(pos);
			sprite.SetTextureSize(size);

			line_stream >> pos.x;
			line_stream >> pos.y;

			sprite.SetAnchorPoint(pos);

			uint16_t tag;
			line_stream >> tag;
			sprite.SetTags(tag);

			ans.emplace(key, sprite);
		}
	}

	//ファイル閉じる
	file.close();

	return ans;
}

void UIEditor::LoadEditFile()
{
	int ind = (int)filename_.find_last_of('\0');
	if (ind > 0) filename_ = filename_.substr(0, ind);

	sprites_ = LoadFile(filename_);

	editUI_ = true;
}

void UIEditor::SaveFile()
{
	//	編集中じゃなかったら
	if (!editUI_) return;

	int ind = (int)filename_.find_last_of('\0');
	if (ind > 0) filename_ = filename_.substr(0, ind);

	std::string filePath = "Resources/Levels/" + filename_ + ".txt";

	std::ofstream outPutFile;
	outPutFile.open(filePath);
	
	for (auto itr = sprites_.begin(); itr != sprites_.end(); ++itr)
	{
		outPutFile << itr->first << " " << itr->second.GetTexture()->GetTextureName()
			<< " " << itr->second.GetPosition().x << " " << itr->second.GetPosition().y 
			<< " " << itr->second.GetSize().x << " " << itr->second.GetSize().y 
			<< " " << itr->second.GetTextureLeftTop().x << " " << itr->second.GetTextureLeftTop().y
			<< " " << itr->second.GetTextureSize().x << " " << itr->second.GetTextureSize().y
			<< " " << itr->second.GetAnchorPoint().x << " " << itr->second.GetAnchorPoint().y
			<< " " << itr->second.GetTags() << std::endl;
	}

	outPutFile.close();

	CloseEditer();
}

void UIEditor::CloseEditer()
{
	editUI_ = false;
	sprites_.clear();
}

void UIEditor::DeleteSpriteForList()
{
	for (auto itr = eraseSpriteName_.begin(); itr != eraseSpriteName_.end(); ++itr)
	{
		sprites_.erase(*itr);
	}
	eraseSpriteName_.clear();
}

void UIEditor::AddSprite()
{
	Sprite sprite;
	sprite.Initialize(nullptr);
	sprites_.emplace(spritename_, sprite);
}

void UIEditor::ReNameSprite(std::map<std::string, Sprite, std::less<>>::iterator& itr)
{
	//	既にある名前だったら
	if (sprites_.count(spritename_) == 1) return;

	Sprite sprite = itr->second;
	eraseSpriteName_.push_back(itr->first);

	int ind = (int)spritename_.find_last_of('\0');
	if (ind > 0) spritename_ = spritename_.substr(0, ind);

	sprites_.emplace(spritename_, sprite);
}

void UIEditor::DrawSpriteInfo(std::map<std::string, Sprite, std::less<>>::iterator& itr)
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	if (imguiMan->CollapsingHeader(itr->first))
	{
		Sprite* sprite = &itr->second;

		if (imguiMan->SetButton("ReName")) {
			ReNameSprite(itr);
			return;
		}
		
		imguiMan->Text("Tag");
		for (size_t i = 0; i < 16; i++)
		{
			if (i % 8 != 0) imguiMan->SameLine();

			uint16_t tagIdx = 0b1 << i;
			bool tagFlag = sprite->GetTags() & tagIdx;
			bool prevFlag = tagFlag;
			imguiMan->CheckBox(std::to_string(i), tagFlag);

			if (tagFlag == prevFlag) continue;

			if(tagFlag) sprite->SetTags(sprite->GetTags() | tagIdx);
			else		sprite->SetTags(sprite->GetTags() ^ tagIdx);
		}

		Vector2D vec = sprite->GetPosition();
		imguiMan->SetSliderFloat2("PosLeftTop", vec);
		sprite->SetPosition(vec);

		if (imguiMan->SetButton("SetWinCenterPos")) sprite->SetPosition(Vector2D(Window::sWIN_WIDTH / 2.0f, Window::sWIN_HEIGHT / 2.0f));

		vec = sprite->GetSize();
		imguiMan->SetSliderFloat2("Size", vec);
		sprite->SetSize(vec);

		imguiMan->Text("Texture");
		std::string texName = sprite->GetTexture()->GetTextureName();
		imguiMan->InputText("TexName ", texName);
		if (imguiMan->SetButton("Paste")) sprite->SetHandle(TextureManager::GetInstance()->PasteTexture());

		vec = sprite->GetTextureLeftTop();
		imguiMan->SetSliderFloat2("TexLeftTop", vec);
		sprite->SetTextureLeftTop(vec);

		vec = sprite->GetTextureSize();
		imguiMan->SetSliderFloat2("TexSize ", vec);
		sprite->SetTextureSize(vec);

		vec = sprite->GetAnchorPoint();
		imguiMan->SetSliderFloat2("AnchorPoint ", vec, 0.1f, 0.0f, 1.0f);
		sprite->SetAnchorPoint(vec);

		if (imguiMan->SetButton("Delete")) 	eraseSpriteName_.push_back(itr->first);
	}
}

UIEditor* UIEditor::GetInstance()
{
	static UIEditor instance;
	return &instance;
}

void UIEditor::ImGuiUpdate()
{
	if (!ImGuiController::GetInstance()->GetActiveUIManager()) return;

	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("UIEditor", true);

	if (imguiMan->BeginMenuBar()) {
		if (imguiMan->BeginMenu("File")) {
			if (imguiMan->MenuItem("New")) editUI_ = true;
			if (imguiMan->MenuItem("Save")) SaveFile();
			if (imguiMan->MenuItem("Close")) CloseEditer();
			imguiMan->EndMenu();
		}
		imguiMan->EndMenuBar();
	}

	imguiMan->InputText("FileName", filename_);
	imguiMan->SameLine();
	if (imguiMan->SetButton("Load")) LoadEditFile();

	imguiMan->CheckBox("GlayScale", activeGlayscale_);
	
	imguiMan->Text("DrawTag");
	for (size_t i = 0; i < 16; i++)
	{
		if (i % 8 != 0) imguiMan->SameLine();

		uint16_t tagIdx = 0b1 << i;
		bool tagFlag = drawTag_ & tagIdx;
		bool prevFlag = tagFlag;
		imguiMan->CheckBox(std::to_string(i), tagFlag);

		if (tagFlag == prevFlag) continue;

		if (tagFlag) drawTag_ = drawTag_ | tagIdx;
		else		 drawTag_ = drawTag_ ^ tagIdx;
	}

	if (editUI_) {
		imguiMan->Spacing();
		imguiMan->Separator();
		imguiMan->Spacing();

		imguiMan->InputText("SpriteName", spritename_);
	
		if (imguiMan->SetButton("AddSprite"))	AddSprite();
	
		imguiMan->BeginChild();

		int32_t id = 0;
		for (auto itr = sprites_.begin(); itr != sprites_.end(); ++itr)
		{
			imguiMan->PushID(id++);

			DrawSpriteInfo(itr);

			imguiMan->Spacing();

			imguiMan->PopID();
		}

		DeleteSpriteForList();

		imguiMan->EndChild();
	}

	imguiMan->EndWindow();

	for (auto& sprite : sprites_) {
		sprite.second.Update();
	}
}

void UIEditor::DrawEditUI()
{
	if (!editUI_) return;

	for (auto& sprite : sprites_) {
		if (sprite.second.GetTags() & drawTag_) {
			sprite.second.Draw();
		}
	}
}

void UIEditor::Draw()
{
	DrawEditUI();
}
