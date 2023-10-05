#pragma once
#include "UIButton.h"
#include <map>
#include <cstdint>
#include <list>
#include <fstream>

class UIButtonManager
{
private:
	std::string buttonName_;
	std::list<std::string> eraseButtonName_;

	std::map<std::string, UIButton, std::less<>> buttons_;

	//int16_t activeButtonNum_ = 0;
	int16_t selectButtonNum_ = 0;

	//	activeButtonnamelist
	std::vector<std::string> activeNameArry;
private:
	void AddButton();
	void ButtonInfo(std::map<std::string, UIButton, std::less<>>::iterator& itr);
	void RenameButton(std::map<std::string, UIButton, std::less<>>::iterator& itr);
	void SetButtonTag(UIButton* pButton);

public:
	void Update(int16_t inputValue);	//	入力値-1~1
	void ImGuiUpdate(int32_t& id);
	void SaveData(std::ofstream& outPutFile);
	void LoadUIButton(UIButton& button, const std::string& name);

	void MatUpdate();
	void Draw();

	const std::string& GetSelectName();

	void SetNumber(uint16_t drawTag);
};

