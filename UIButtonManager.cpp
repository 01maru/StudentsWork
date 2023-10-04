#include "UIButtonManager.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include "TextureManager.h"

void UIButtonManager::AddButton()
{
	UIButton button;
	button.Initialize(nullptr);
	button.SetAnchorPoint({ 0.5f,0.5f });
	buttons_.emplace(buttonName_, button);
}

void UIButtonManager::RenameButton(std::map<std::string, UIButton, std::less<>>::iterator& itr)
{
	//	既にある名前だったら
	if (buttons_.count(buttonName_) == 1) return;

	UIButton button = itr->second;
	//	前の名前のボタンは削除
	eraseButtonName_.push_back(itr->first);

	int ind = (int)buttonName_.find_last_of('\0');
	if (ind > 0) buttonName_ = buttonName_.substr(0, ind);
	//	新しい名前のボタン生成
	buttons_.emplace(buttonName_, button);
}

void UIButtonManager::SetButtonTag(UIButton* pButton)
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->Text("Tag");
	for (size_t i = 0; i < 16; i++)
	{
		if (i % 8 != 0) imguiMan->SameLine();

		uint16_t tagIdx = 0b1 << i;
		bool tagFlag = pButton->GetTags() & tagIdx;
		bool prevFlag = tagFlag;
		imguiMan->CheckBox(std::to_string(i), tagFlag);

		if (tagFlag == prevFlag) continue;

		if (tagFlag) pButton->SetTags(pButton->GetTags() | tagIdx);
		else		pButton->SetTags(pButton->GetTags() ^ tagIdx);
	}
}

void UIButtonManager::ButtonInfo(std::map<std::string, UIButton, std::less<>>::iterator& itr)
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	if (imguiMan->CollapsingHeader(itr->first))
	{
		UIButton* button = &itr->second;

		if (imguiMan->SetButton("Rename")) {
			RenameButton(itr);
			return;
		}

		SetButtonTag(button);

		Vector2D vec = button->GetPosition();
		imguiMan->SetSliderFloat2("CenterPos", vec);
		button->SetPosition(vec);

		vec = button->GetSize();
		imguiMan->SetSliderFloat2("Size", vec);
		button->SetSize(vec);

		imguiMan->Text("Texture");
		std::string texName = button->GetTexture()->GetTextureName();
		imguiMan->InputText("TexName ", texName);
		if (imguiMan->SetButton("Paste")) button->SetHandle(TextureManager::GetInstance()->PasteTexture());

		if (imguiMan->SetButton("Delete")) 	eraseButtonName_.push_back(itr->first);
	}
}

void UIButtonManager::Update(int16_t inputValue)
{
	//	選択するボタンが複数なかったら
	int32_t arrySize = (int32_t)activeNameArry.size();
	if (arrySize <= 1) return;

	selectButtonNum_ += inputValue;
	MyMath::mLoop(arrySize, 1, selectButtonNum_);

	for (size_t i = 0; i < arrySize; i++)
	{
		//buttons_[activeNameArry[i]].SetIsSelect(i == selectButtonNum_);
	}
}

void UIButtonManager::ImGuiUpdate(int32_t& id)
{
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->Spacing();
	imguiMan->Separator();
	imguiMan->Spacing();

	imguiMan->InputText("ButtonName", buttonName_);

	if (imguiMan->SetButton("AddButton"))	AddButton();

	for (auto itr = buttons_.begin(); itr != buttons_.end(); ++itr)
	{
		imguiMan->PushID(id++);

		ButtonInfo(itr);

		imguiMan->Spacing();

		imguiMan->PopID();
	}
}

void UIButtonManager::SaveData(std::ofstream& outPutFile)
{
	for (auto itr = buttons_.begin(); itr != buttons_.end(); ++itr)
	{
		outPutFile << "B " << itr->first << " " << itr->second.GetTexture()->GetTextureName()
			<< " " << itr->second.GetPosition().x << " " << itr->second.GetPosition().y
			<< " " << itr->second.GetSize().x << " " << itr->second.GetSize().y
			<< " " << itr->second.GetTags() << std::endl;
	}
}

void UIButtonManager::LoadUIButton(UIButton& button, const std::string& name)
{
	buttons_.emplace(name, button);
}

void UIButtonManager::MatUpdate()
{
	for (size_t i = 0; i < activeNameArry.size(); i++)
	{
		buttons_[activeNameArry[i]].Update();
	}
}

void UIButtonManager::Draw()
{
	for (size_t i = 0; i < activeNameArry.size(); i++)
	{
		buttons_[activeNameArry[i]].Draw();
	}
}

void UIButtonManager::SetNumber(uint16_t drawTag)
{
	activeNameArry.clear();

	selectButtonNum_ = 0;

	for (auto itr = buttons_.begin(); itr != buttons_.end(); ++itr)
	{
		UIButton& button = itr->second;
		//	描画されていたら
		if (button.GetTags() & drawTag) {
			activeNameArry.push_back(itr->first);
		}
	}
}
