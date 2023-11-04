#pragma once
#include "Sprite.h"
#include "UIData.h"
#include <unordered_map>
#include <map>
#include <list>

/**
* @file UIEditor.h
* @brief UIの画面表示位置などを編集できる機能をまとめたファイル
*/

#pragma region 前置宣言

class UIAnimationTimer;

#pragma endregion

class UIEditor :public UIData
{
public:
	static UIEditor* GetInstance();
	UIEditor(const UIEditor& obj) = delete;
	UIEditor& operator=(const UIEditor& obj) = delete;

	UIEditor() {};
	~UIEditor() {};

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

private:
	std::string filename_;
	std::string spritename_;
	std::string objName_;
	std::string tagname_;
	std::list<std::string> eraseSpriteName_;

	uint16_t drawTag_ = 0;

	bool activeGlayscale_ = false;
	bool editUI_ = false;
	bool editAnimation_ = false;

private:
	/**
	* @fn LoadEditFileData()
	* 編集する用のUIData読み込み用関数
	*/
	void LoadEditFileData();
	/**
	* @fn SaveFileData()
	* 編集したUIDataを保存する用関数
	*/
	void SaveFileData();

	/**
	* @fn CloseEditer()
	* Editer終了時関数
	*/
	void CloseEditer();
	void EditUIAnimationTimer(UIAnimationTimer* pComponent);
	void DrawUIAnimationTimerInfo(UIAnimationTimer* pComponent);
	bool ImGuiUpdateUIAnimationTimer(std::unordered_map<std::string, std::unique_ptr<UIObject>>::iterator& itr);
	
	void ImGuiUpdateMoveAnimation(std::unordered_map<std::string, std::unique_ptr<UIObject>>::iterator& itr);
	void ImGuiObjAnimation(std::unordered_map<std::string, std::unique_ptr<UIObject>>::iterator& itr, int32_t& id);
#pragma region UIData::count_

	void CreateAnimationCount();
	void DeleteAnimationCount();
	void AnimationImGuiUpdate();

#pragma endregion


	void AddTag();
	void DeleteTag();
	void EditTag();
	void DeleteSpriteFromList(std::unordered_map<std::string, Sprite>& sprites);
	void AddSprite(std::unordered_map<std::string, Sprite>& sprites);
	void ReNameSprite(std::unordered_map<std::string, Sprite>& sprites, std::pair<const std::string, Sprite> data);
	void ImGuiSpriteInfo(std::unordered_map<std::string, std::unique_ptr<UIObject>>::iterator& itr, int32_t& id);

	void AddUIObject();
	void EditUIObject(std::unordered_map<std::string, std::unique_ptr<UIObject>>::iterator& itr, int32_t& id);
public:

#pragma region Getter

	/**
	* @fn GetActiveGlayscale()
	* activeGlayscale_のGetter関数
	* @return activeGlayscale_の値
	*/
	bool GetActiveGlayscale();
	/**
	* @fn GetEditUI()
	* editUI_のGetter関数
	* @return editUI_の値
	*/
	bool GetEditUI();

#pragma endregion
};

