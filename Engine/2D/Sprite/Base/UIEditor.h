#pragma once
#include "Sprite.h"
#include "UIData.h"
#include <map>
#include <list>

/**
* @file UIEditor.h
* @brief UIの画面表示位置などを編集できる機能をまとめたファイル
*/

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

	/**
	* @fn ImGuiUpdate()
	* ImGui更新処理関数
	*/
	void ImGuiUpdate();
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw();

#pragma region Getter

	/**
	* @fn GetActiveGlayscale()
	* activeGlayscale_のGetter関数
	* @return activeGlayscale_の値
	*/
	bool GetActiveGlayscale() { return activeGlayscale_; }
	/**
	* @fn GetEditUI()
	* editUI_のGetter関数
	* @return editUI_の値
	*/
	bool GetEditUI() { return editUI_; }

#pragma endregion

	/**
	* @fn LoadUIData(const std::string&)
	* レベルデータ読み込み用関数
	* @param filename レベルデータのファイル名
	*/
	std::unique_ptr<UIData> LoadUIData(const std::string& filename);
};

