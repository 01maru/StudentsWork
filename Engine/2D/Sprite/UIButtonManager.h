//#pragma once
//#include "UIButton.h"
//#include <map>
//#include <unordered_map>
//#include <cstdint>
//#include <list>
//#include <fstream>
//
///**
//* @file UIButtonManager.h
//* @brief UIEditor用のButtonをまとめて管理したファイル
//*/
//
//class UIButtonManager
//{
//private:
//	std::string buttonName_;
//	std::list<std::string> eraseButtonName_;
//
//	std::unordered_map<std::string, UIButton> buttons_;
//
//	//int16_t activeButtonNum_ = 0;
//	int16_t selectButtonNum_ = 0;
//
//	//	activeButtonnamelist
//	std::vector<std::string> activeNameArry;
//	//std::map<int16_t, std::string> activeNameArray;
//private:
//	void AddButton();
//	void ButtonInfo(std::unordered_map<std::string, UIButton>::iterator& itr);
//	void RenameButton(std::unordered_map<std::string, UIButton>::iterator& itr);
//	void SetButtonTag(UIButton* pButton);
//
//public:
//	void Initialize();
//	/**
//	* @fn Update(int16_t)
//	* 更新処理関数
//	* @param inputValue 入力値(-1～1)
//	*/
//	void Update(int16_t inputValue);
//	/**
//	* @fn ImGuiUpdate(int32_t&)
//	* ImGui更新処理関数
//	* @param id ImGui用のid
//	*/
//	void ImGuiUpdate(int32_t& id);
//	/**
//	* @fn MatUpdate()
//	* 行列更新処理関数
//	*/
//	void MatUpdate();
//	/**
//	* @fn Draw()
//	* 描画処理関数
//	*/
//	void Draw();
//
//	void SaveData(std::ofstream& outPutFile);
//	void LoadUIButton(UIButton& button, const std::string& name);
//
//	const std::string& GetSelectName();
//
//	void SetNumber(uint16_t drawTag);
//
//	const Vector2D& GetSelectPosition() { return buttons_[activeNameArry[selectButtonNum_]].GetPosition(); }
//};
//
