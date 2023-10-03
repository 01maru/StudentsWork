#pragma once
#include "Sprite.h"
#include "UIData.h"
#include <map>
#include <list>

class UIEditor
{
private:
	std::string filename_;
	std::string spritename_;
	std::string tagname_;
	std::list<std::string> eraseSpriteName_;

	uint16_t drawTag_ = 0;

	std::unique_ptr<UIData> data_;

	bool activeGlayscale_ = false;
	bool editUI_ = false;
private:
	UIEditor() {};
	~UIEditor() {};

	void AddTag();
	void DeleteTag();
	void LoadEditFile();
	void SaveFile();
	void CloseEditer();
	void EditTag();
	void DeleteSpriteFromList();
	void AddSprite();
	void ReNameSprite(std::map<std::string, Sprite, std::less<>>::iterator& itr);
	void DrawSpriteInfo(std::map<std::string, Sprite, std::less<>>::iterator& itr);
	void DrawEditUI();
	void LoadFile(const std::string& filename);
public:
	static UIEditor* GetInstance();
	UIEditor(const UIEditor& obj) = delete;
	UIEditor& operator=(const UIEditor& obj) = delete;

	void ImGuiUpdate();
	void Draw();

	bool GetActiveGlayscale() { return activeGlayscale_; }
	bool GetEditUI() { return editUI_; }

	std::unique_ptr<UIData> LoadUIData(const std::string& filename);
};

