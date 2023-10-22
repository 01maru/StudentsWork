#include "UIEditor.h"
#include "ImGuiManager.h"
#include "ImGuiController.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include "TextureManager.h"
#include "Window.h"
#include "ConvertString.h"
#include "UISprite.h"
#include "UIAnimationTimer.h"
#include "UIMoveAnimation.h"

UIEditor* UIEditor::GetInstance()
{
	static UIEditor instance;
	return &instance;
}

void UIEditor::LoadEditFileData()
{
	int ind = (int)filename_.find_last_of('\0');
	if (ind > 0) filename_ = filename_.substr(0, ind);

	LoadData(filename_);

	editUI_ = true;
}

void UIEditor::SaveFileData()
{
	//	編集中じゃなかったら
	if (!editUI_) return;

	int ind = (int)filename_.find_last_of('\0');
	if (ind > 0) filename_ = filename_.substr(0, ind);

	std::string filePath = "Resources/Levels/" + filename_ + ".txt";

	std::ofstream outPutFile;
	outPutFile.open(filePath);

	//	全体のTimer保存
	if (count_ != nullptr) {
		outPutFile << "Timer "
			<< count_->GetMaxFrameCount() << "\n" << std::endl;
	}
	
	//	UIObject保存
	for (auto itr = obj_.begin(); itr != obj_.end(); ++itr)
	{
		UIAnimationTimer* timer = itr->second->GetComponent<UIAnimationTimer>();
		UIMoveAnimation* move = itr->second->GetComponent<UIMoveAnimation>();

		//	Spriteの保存
		UISprite* sprite = itr->second->GetComponent<UISprite>();
		if (sprite != nullptr) {
			for (auto itrsprite : sprite->GetSprites())
			{
				outPutFile << "S " << itrsprite.first << " " << itrsprite.second.GetTexture()->GetTextureName()
					<< " " << itrsprite.second.GetPosition().x << " " << itrsprite.second.GetPosition().y
					<< " " << itrsprite.second.GetSize().x << " " << itrsprite.second.GetSize().y
					<< " " << itrsprite.second.GetTextureLeftTop().x << " " << itrsprite.second.GetTextureLeftTop().y
					<< " " << itrsprite.second.GetTextureSize().x << " " << itrsprite.second.GetTextureSize().y
					<< " " << itrsprite.second.GetAnchorPoint().x << " " << itrsprite.second.GetAnchorPoint().y
					<< " " << itrsprite.second.GetTags() << std::endl;
			}
		}

		////	Buttonの保存
		//if (!=nullptr) {

		//}

		////	Sliderの保存
		//if (!=nullptr) {

		//}

		//	UIAnimationTimerの保存
		if (timer != nullptr) {
			outPutFile << "AnimeTimer " << timer->GetMaxFrameCount()
				<< " " << timer->GetStartCount() << std::endl;
		}

		//	UIMoveAnimationの保存
		if (move != nullptr) {
			outPutFile << "MoveAnime " 
				<< move->GetStartPos().x << " "
				<< move->GetStartPos().y << " "
				<< move->GetEndPos().x   << " "
				<< move->GetEndPos().y
				<< std::endl;
		}

		outPutFile << "EndData " << itr->first
			<< "\n" << std::endl;
	}

	//	SaveTags
	for (auto itr = tagName_.begin(); itr != tagName_.end(); ++itr)
	{
		outPutFile << "T " << itr->first << " " << itr->second << std::endl;
	}

	buttonMan_.SaveData(outPutFile);

	outPutFile.close();

	CloseEditer();
}

void UIEditor::CloseEditer()
{
	editUI_ = false;
	UIData::Finalize();
}

void UIEditor::DeleteSpriteFromList(std::unordered_map<std::string, Sprite>& sprites)
{
	if (eraseSpriteName_.size() == 0) return;

	for (auto itr = eraseSpriteName_.begin(); itr != eraseSpriteName_.end(); ++itr)
	{
		sprites.erase(*itr);
	}
	eraseSpriteName_.clear();
}

void UIEditor::AddSprite(std::unordered_map<std::string, Sprite>& sprites)
{
	Sprite sprite;
	sprite.Initialize(nullptr);
	sprites.emplace(spritename_, sprite);
}

void UIEditor::ReNameSprite(std::unordered_map<std::string, Sprite>& sprites, std::pair<const std::string, Sprite> data)
{
	//	既にある名前だったら
	if (sprites.count(spritename_) == 1) return;

	Sprite sprite = data.second;
	eraseSpriteName_.push_back(data.first);

	int ind = (int)spritename_.find_last_of('\0');
	if (ind > 0) spritename_ = spritename_.substr(0, ind);

	sprites.emplace(spritename_, sprite);
}

void UIEditor::ImGuiSpriteInfo(std::map<std::string, std::unique_ptr<UIObject>, std::less<>>::iterator& itr, int32_t& /*id*/)
{
	UISprite* pComponent = itr->second->GetComponent<UISprite>();
	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	if (pComponent == nullptr) {
		if (imguiMan->SetButton("AddSpriteComponent")) {
			pComponent = itr->second->AddComponent<UISprite>();
		}
		else {
			return;
		}
	}

	if (imguiMan->CollapsingHeader(itr->first + "Sprite"))
	{
		imguiMan->InputText("SpriteName", spritename_);
		if (imguiMan->SetButton("AddSprite")) {
			AddSprite(pComponent->GetSprites());
		}

		for (auto& s : pComponent->GetSprites())
		{
			Sprite* sprite = &s.second;

			//imguiMan->PushID(id++);

			imguiMan->Text(s.first.c_str());
			imguiMan->SameLine();
			if (imguiMan->SetButton("ReName")) {
				ReNameSprite(pComponent->GetSprites(), s);
				return;
			}

			imguiMan->Text("Tag");
			for (size_t i = 0; i < 16; i++)
			{
				if (i % 8 != 0) imguiMan->SameLine();

				uint16_t tagIdx = 0b1 << i;
				bool tagFlag = sprite->GetTags() & tagIdx;
				bool prevFlag = tagFlag;
				imguiMan->CheckBox(std::to_string(i), tagFlag);

				if (tagFlag == prevFlag) continue;

				if (tagFlag) sprite->SetTags(sprite->GetTags() | tagIdx);
				else		sprite->SetTags(sprite->GetTags() ^ tagIdx);
			}

			Vector2D vec = sprite->GetPosition();
			imguiMan->SetSliderFloat2("PosLeftTop", vec);
			sprite->SetPosition(vec);

			if (imguiMan->SetButton("SetWinCenterPos")) sprite->SetPosition(Vector2D(Window::sWIN_WIDTH / 2.0f, Window::sWIN_HEIGHT / 2.0f));

			vec = sprite->GetSize();
			imguiMan->SetSliderFloat2("Size", vec);
			sprite->SetSize(vec);

			imguiMan->Text("Texture");
			std::string texName = sprite->GetTexture()->GetTextureName();
			imguiMan->InputText("TexName ", texName);
			if (imguiMan->SetButton("Paste")) sprite->SetHandle(TextureManager::GetInstance()->PasteTexture());

			vec = sprite->GetTextureLeftTop();
			imguiMan->SetSliderFloat2("TexLeftTop", vec);
			sprite->SetTextureLeftTop(vec);

			vec = sprite->GetTextureSize();
			imguiMan->SetSliderFloat2("TexSize ", vec);
			sprite->SetTextureSize(vec);

			vec = sprite->GetAnchorPoint();
			imguiMan->SetSliderFloat2("AnchorPoint ", vec, 0.1f, 0.0f, 1.0f);
			sprite->SetAnchorPoint(vec);

			if (imguiMan->SetButton("Delete")) 	eraseSpriteName_.push_back(s.first);

			imguiMan->Spacing();
			imguiMan->Separator();
			imguiMan->Spacing();

			//imguiMan->PopID();
		}

		DeleteSpriteFromList(pComponent->GetSprites());
	}
}

void UIEditor::AddUIObject()
{
	std::unique_ptr<UIObject> object = std::make_unique<UIObject>();
	obj_.emplace(objName_, std::move(object));
}

void UIEditor::EditUIAnimationTimer(UIAnimationTimer* pComponent)
{
	if (pComponent->GetIsActive()) return;

	ImGuiManager* imgui = ImGuiManager::GetInstance();

	int32_t count = pComponent->GetStartCount();
	int32_t prevCount = count;
	imgui->InputInt("StartCount", count);
	if (count != prevCount) pComponent->SetStartCount(count);

	count = pComponent->GetMaxFrameCount();
	prevCount = count;
	imgui->InputInt("MaxCount", count);
	if (count != prevCount) pComponent->SetMaxFrameCount(count);
}

void UIEditor::DrawUIAnimationTimerInfo(UIAnimationTimer* pComponent)
{
	if (pComponent->GetIsActive() == false) return;

	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->Text("StartCount : %d", pComponent->GetStartCount());
	imgui->Text("MaxCount : %d", pComponent->GetMaxFrameCount());
	imgui->Text("Count : %d", pComponent->GetFrameCount());
}

bool UIEditor::ImGuiUpdateUIAnimationTimer(std::map<std::string, std::unique_ptr<UIObject>, std::less<>>::iterator& itr)
{
	UIAnimationTimer* pComponent = itr->second->GetComponent<UIAnimationTimer>();
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	if (pComponent == nullptr) {
		if (imgui->SetButton("AddAnimation")) {
			if (count_ == nullptr) {
				CreateAnimationCount();
			}
			pComponent = itr->second->AddComponent<UIAnimationTimer>();
			pComponent->Initialize();
			itr->second->SetCount(count_.get());
		}
		else {
			return false;
		}
	}

	EditUIAnimationTimer(pComponent);
	DrawUIAnimationTimerInfo(pComponent);

	if (imgui->SetButton("CountIsActive")) pComponent->StartCount();
	imgui->SameLine();
	imgui->Text("IsActive : %s", pComponent->GetIsActive() ? "TRUE" : "FALSE");

	imgui->Separator();

	return true;
}

void UIEditor::ImGuiUpdateMoveAnimation(std::map<std::string, std::unique_ptr<UIObject>, std::less<>>::iterator& itr)
{
	UIMoveAnimation* pComponent = itr->second->GetComponent<UIMoveAnimation>();
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	if (pComponent == nullptr) {
		if (imgui->SetButton("AddMove")) {
			UIMoveAnimation* uiMove = itr->second->AddComponent<UIMoveAnimation>();
			uiMove->Initialize();
		}
		else {
			return;
		}
	}

	Vector2D pos = pComponent->GetStartPos();
	Vector2D prevPos = pos;
	imgui->InputFloat2("StartPos", pos);
	if (prevPos != pos) pComponent->SetStartPos(pos);

	pos = pComponent->GetEndPos();
	prevPos = pos;
	imgui->InputFloat2("EndPos", pos);
	if (prevPos != pos) pComponent->SetEndPos(pos);

	imgui->Separator();
}

void UIEditor::ImGuiObjAnimation(std::map<std::string, std::unique_ptr<UIObject>, std::less<>>::iterator& itr, int32_t& /*id*/)
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	if (imgui->CollapsingHeader("Animation"))
	{
		//imgui->PushID(id++);

		if (ImGuiUpdateUIAnimationTimer(itr) == false) return;

		ImGuiUpdateMoveAnimation(itr);

		//imgui->PopID();
	}
}

void UIEditor::EditUIObject(std::map<std::string, std::unique_ptr<UIObject>, std::less<>>::iterator& itr, int32_t& id)
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->PushID(id++);

	if (imgui->CollapsingHeader(itr->first))
	{
		ImGuiSpriteInfo(itr, id);

		ImGuiObjAnimation(itr, id);
	}

	imgui->PopID();

	imgui->Spacing();
	imgui->Separator();
	imgui->Spacing();
}

void UIEditor::AddTag()
{
	if (tagName_.count(tagname_) == 1) {
		tagName_[tagname_] = drawTag_;
		return;
	}

	tagName_.emplace(tagname_, drawTag_);
}

void UIEditor::DeleteTag()
{
	tagName_.erase(tagname_);
}

void UIEditor::EditTag()
{
	if (!editUI_) return;

	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->Spacing();
	imguiMan->Separator();
	imguiMan->Spacing();

	imguiMan->Text("DrawTag");
	for (size_t i = 0; i < 16; i++)
	{
		if (i % 8 != 0) imguiMan->SameLine();

		uint16_t tagIdx = 0b1 << i;
		bool tagFlag = drawTag_ & tagIdx;
		bool prevFlag = tagFlag;
		imguiMan->CheckBox(std::to_string(i), tagFlag);

		if (tagFlag == prevFlag) continue;

		if (tagFlag) drawTag_ = drawTag_ | tagIdx;
		else		 drawTag_ = drawTag_ ^ tagIdx;
	}

	imguiMan->InputText("TagName", tagname_);
	if (imguiMan->SetButton("Add")) { AddTag(); }
	imguiMan->SameLine();
	if (imguiMan->SetButton("Delete")) { DeleteTag(); }

	//	Tag一覧表示
	if (imguiMan->CollapsingHeader("Tags")) {
		for (auto itr = tagName_.begin(); itr != tagName_.end(); ++itr)
		{
			if (imguiMan->SetButton(itr->first.c_str())) drawTag_ = tagName_[itr->first];
		}
	}

	//	buttonTag設定
	buttonMan_.SetNumber(drawTag_);
}

void UIEditor::ImGuiUpdate()
{
	if (!ImGuiController::GetInstance()->GetActiveUIManager()) return;

	ImGuiManager* imguiMan = ImGuiManager::GetInstance();

	imguiMan->BeginWindow("UIEditor", true);

	if (imguiMan->BeginMenuBar()) {
		if (imguiMan->BeginMenu("File")) {
			if (imguiMan->MenuItem("New")) editUI_ = true;
			if (imguiMan->MenuItem("Save")) SaveFileData();
			if (imguiMan->MenuItem("Close")) CloseEditer();
			imguiMan->EndMenu();
		}
		imguiMan->EndMenuBar();
	}

	imguiMan->InputText("FileName", filename_);
	imguiMan->SameLine();
	if (imguiMan->SetButton("Load")) LoadEditFileData();

	imguiMan->CheckBox("GlayScale", activeGlayscale_);

	imguiMan->CheckBox("Animation", editAnimation_);
	
	EditTag();

	if (editUI_) {
		imguiMan->Spacing();
		imguiMan->Separator();
		imguiMan->Spacing();

		int32_t id = 0;

		imguiMan->InputText("ObjectName", objName_);
		if (imguiMan->SetButton("AddObject")) {
			AddUIObject();
		}

		if (imguiMan->BeginChild()) {

			for (auto itr = obj_.begin(); itr != obj_.end(); ++itr)
			{
				EditUIObject(itr, id);

				imguiMan->Spacing();
			}

			imguiMan->EndChild();
		}

		buttonMan_.ImGuiUpdate(id);

	}

	imguiMan->EndWindow();

	if (!editUI_) return;

	AnimationImGuiUpdate();

	UIData::Update();
}

void UIEditor::CreateAnimationCount()
{
	count_ = std::make_unique<FrameCounter>();
	count_->Initialize(60, true);
}

void UIEditor::DeleteAnimationCount()
{
	count_.release();
	editAnimation_ = false;
}

void UIEditor::AnimationImGuiUpdate()
{
	if (editAnimation_ == false) return;

	if (count_ == nullptr) CreateAnimationCount();

	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->BeginWindow("Animation", true);

	imgui->Text("Timer : %d", count_->GetFrameCount());

	int32_t time = count_->GetMaxFrameCount();
	imgui->InputInt("Time", time);
	count_->SetMaxFrameCount(time);

	if (imgui->SetButton("StartTimer")) count_->StartCount();

	if (imgui->SetButton("DeleteCount")) DeleteAnimationCount();

	imgui->EndWindow();
}

void UIEditor::Draw()
{
	if (!editUI_) return;

	UIData::Draw();
}
