#pragma once
#include "Sprite.h"
#include <map>
#include <list>

class UIEditor
{
private:
	std::string filename_;
	std::string spritename_;

	uint16_t drawTag_ = 0;

	std::map<std::string, Sprite, std::less<>> sprites_;
	std::list<std::string> eraseSpriteName_;

	bool activeGlayscale_ = false;
	bool editUI_ = false;
private:
	UIEditor() {};
	~UIEditor() {};

	void LoadEditFile();
	void SaveFile();
	void CloseEditer();
	void DeleteSpriteForList();
	void AddSprite();
	void ReNameSprite(std::map<std::string, Sprite, std::less<>>::iterator& itr);
	void DrawSpriteInfo(std::map<std::string, Sprite, std::less<>>::iterator& itr);
	void DrawEditUI();
public:
	static UIEditor* GetInstance();
	UIEditor(const UIEditor& obj) = delete;
	UIEditor& operator=(const UIEditor& obj) = delete;

	void ImGuiUpdate();
	void Draw();

	bool GetActiveGlayscale() { return activeGlayscale_; }
	bool GetEditUI() { return editUI_; }
	std::map<std::string, Sprite, std::less<>> LoadFile(const std::string& filename);
};

