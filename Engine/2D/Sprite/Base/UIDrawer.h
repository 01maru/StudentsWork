#pragma once
#include "UIData.h"

class UIDrawer
{
private:
	//uint16_t activeTag_ = 0b1111111111111111;
	std::string activeTagName_;

	std::unique_ptr<UIData> data_;

public:
	void Update(int16_t inputValue);
	void Draw();

	const std::string& GetActiveTagName() { return activeTagName_; }

	void SetUI(const std::string& uiName);
	void LoadSprites(const std::string& filename);
	void LoadData(const std::string& filename);
};

