#pragma once
#include "UIButton.h"
#include <map>
#include <cstdint>

class UIButtonManager
{
private:
	std::map<std::string, UIButton, std::less<>> buttons_;

	int16_t activeButtonNum_;
	int16_t selectButtonNum_;

public:
	void Initialize();
	void Update(int16_t inputValue);	//	“ü—Í’l-1~1

	void AddButton();
};

