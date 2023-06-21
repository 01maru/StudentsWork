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

	void LoadFile();
	void SaveFile();
	void DeleteSpriteForList();
	void AddSprite();
	void ReNameSprite(std::map<std::string, Sprite, std::less<>>::iterator& itr);
	void DrawSpriteInfo(std::map<std::string, Sprite, std::less<>>::iterator& itr);
public:
	static UIManager* GetInstance();
	UIManager(const UIManager& obj) = delete;
	UIManager& operator=(const UIManager& obj) = delete;

	void Initialize();
	void ImGuiUpdate();
	void Draw();
};

