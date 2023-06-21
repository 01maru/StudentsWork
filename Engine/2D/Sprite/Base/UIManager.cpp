#include "UIManager.h"
#include "ImGuiManager.h"
#include "ImGuiController.h"
#include <iostream>
#include <fstream>
#include <sstream>

void UIManager::LoadFile()
{
	std::string filePath = "Resources/Levels/" + filename_ + ".txt";

	//ファイル開く(開けなかったら新規作成)
	std::ifstream file;
	file.open(filePath.c_str(), std::ios_base::app);

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
			sprite.Initialize(nullptr);

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

			sprites_.emplace(key, sprite);
		}
	}

	//ファイル閉じる
	file.close();

	editUI_ = true;
}

void UIManager::SaveFile()
{
	//	編集中じゃなかったら
	if (!editUI_) return;

	std::string filePath = "Resources/Levels/" + filename_ + ".txt";

	std::ofstream outPutFile;
	outPutFile.open(filePath, std::ios_base::app);

	for (auto itr = sprites_.begin(); itr != sprites_.end(); ++itr)
	{
		outPutFile << itr->first << " " << itr->second.GetTexture()->GetTextureName()
			<< " " << itr->second.GetPosition().x << " " << itr->second.GetPosition().y 
			<< " " << itr->second.GetSize().x << " " << itr->second.GetSize().y 
			<< " " << itr->second.GetTextureLeftTop().x << " " << itr->second.GetTextureLeftTop().y
			<< " " << itr->second.GetTextureSize().x << " " << itr->second.GetTextureSize().y << std::endl;
	}

	outPutFile.close();

	editUI_ = false;
}

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

	int ind = (int)spritename_.find_last_of('\0');
	if (ind > 0) spritename_ = spritename_.substr(0, ind);

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
			if (imguiMan->MenuItem("New")) editUI_ = true;
			if (imguiMan->MenuItem("Load")) LoadFile();
			if (imguiMan->MenuItem("Save")) SaveFile();
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
