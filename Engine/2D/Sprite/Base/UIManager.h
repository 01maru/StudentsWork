#pragma once
#include "Sprite.h"
#include <map>
#include <list>

class UIManager
{
private:
	std::string filename_ = "Filename";
	std::string spritename_ = "Spritename";

	std::map<std::string, Sprite, std::less<>> sprites_;
	std::list<std::string> eraseSpriteName_;

	bool activeGlayscale_ = false;
	bool editUI_ = false;
private:
	UIManager() {};
	~UIManager() {};

	void LoadEditFile();
	void SaveFile();
	void CloseEditer();
	void DeleteSpriteForList();
	void AddSprite();
	void ReNameSprite(std::map<std::string, Sprite, std::less<>>::iterator& itr);
	void DrawSpriteInfo(std::map<std::string, Sprite, std::less<>>::iterator& itr);
	void DrawEditUI();
public:
	static UIManager* GetInstance();
	UIManager(const UIManager& obj) = delete;
	UIManager& operator=(const UIManager& obj) = delete;

	void ImGuiUpdate();
	void Draw();

	bool GetActiveGlayscale() { return activeGlayscale_; }
	bool GetEditUI() { return editUI_; }
	std::map<std::string, Sprite, std::less<>> LoadFile(const std::string& filename);
};

