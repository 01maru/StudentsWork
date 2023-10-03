#include "UIButtonManager.h"
#include "MyMath.h"

void UIButtonManager::Update(int16_t inputValue)
{
	//	‘I‘ğ‚·‚éƒ{ƒ^ƒ“‚ª•¡”‚È‚©‚Á‚½‚ç
	if (activeButtonNum_ <= 1) return;

	selectButtonNum_ += inputValue;
	MyMath::mLoop(activeButtonNum_, 1, selectButtonNum_);

	int16_t buttonNum = 0;
	for (auto itr = buttons_.begin(); itr != buttons_.end(); ++itr)
	{
		UIButton* button = &itr->second;

		if (!button->GetIsActive()) continue;
		
		bool select = button->GetNumber() == selectButtonNum_;
		button->SetIsActive(select);

		if (activeButtonNum_ >= ++buttonNum) break;
	}
}
