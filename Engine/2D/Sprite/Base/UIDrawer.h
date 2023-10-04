#pragma once
#include "UIData.h"

class UIDrawer
{
private:
	std::string activeTagName_;

	std::unique_ptr<UIData> data_;

public:
	void Update(int16_t inputValue);
	void Draw();

	const std::string& GetActiveTagName() { return activeTagName_; }
	const std::string& GetActiveButtonName() { return data_->buttonMan_.GetSelectName(); }

	void SetUI(const std::string& uiName);
	void LoadSprites(const std::string& filename);
	void LoadData(const std::string& filename);
};

