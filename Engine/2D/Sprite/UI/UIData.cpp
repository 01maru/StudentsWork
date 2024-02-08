#include "UIData.h"
#include <fstream>
#include <sstream>
#include "TextureManager.h"
#include "ConvertString.h"
#include "UIAnimationTimer.h"
#include "UIMoveAnimation.h"
#include "UIFadeAnimation.h"
#include "UIPosition.h"
#include "UIButton.h"
#include "UISprite.h"
#include "UIRotation.h"
#include "SliderSprite.h"
#include "UIScaling.h"
#include "InputManager.h"

using namespace MyMath;

void MNE::UIData::Finalize()
{
	count_.release();
	obj_.clear();
	tagName_.clear();
}

//-----------------------------------------------------------------------------
// [SECTION] Update
//-----------------------------------------------------------------------------

void MNE::UIData::CollisonCursorUpdate()
{
	for (auto& slider : sliderColliders_)
	{
		slider->ColliderUpdate();
	}

	if (InputManager::GetInstance()->GetUsePad() == FALSE)
	{
		selecting_ = false;
		for (auto& coll : buttonColliders_)
		{
			if (MyMath::CollisionSquareToPoint(coll.leftTop_, coll.rightBottom_, InputManager::GetInstance()->GetMouse()->GetCursor()) == TRUE)
			{
				buttonMan_->SetSelectButton(coll.parent_);
				selecting_ = true;
			}
		}
	}
	else
	{
		selecting_ = TRUE;
	}
}

void MNE::UIData::InputUpdate(int16_t inputValue)
{
	if (InputManager::GetInstance()->GetUsePad() == TRUE)
	{
		buttonMan_->Update(inputValue);
	}
}

void MNE::UIData::Update()
{
	//	オブジェクト更新処理
	for (auto& sprite : obj_) {
		sprite.second->Update();
		sprite.second->MatUpdate();
	}

	//	カウンターがあったら更新処理
	if (count_ != nullptr) {
		count_->Update();
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Draw
//-----------------------------------------------------------------------------

void MNE::UIData::Draw()
{
	//	オブジェクト描画処理
	for (auto& sprite : obj_) {
		//if (sprite.second.GetTags() & titleData_->tagName_[activeTagName_]) {
		sprite.second->Draw();
		//}
	}
}

//-----------------------------------------------------------------------------
// [SECTION] Load
//-----------------------------------------------------------------------------

void MNE::UIData::LoadData(const std::string& filename)
{
	const std::string filePath = "Resources/Levels/" + filename + ".txt";

	//ファイル開く(開けなかったら新規作成 : Editor用)
	std::ifstream file;
	file.open(filePath.c_str());

	std::unique_ptr<UIObject> object;
	std::map<std::string, UIButton*, std::less<>> buttons;
	buttonColliders_.clear();

	// データの上から1行ずつ読み込む
	std::string line;
	while (getline(file, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		if (key == "Timer") {
			if (count_ == nullptr) count_ = std::make_unique<FrameCounter>();

			uint32_t time;

			line_stream >> time;
			count_->Initialize(time, true);
		}

		if (key == "T") {
			std::string tagname;
			uint16_t tagnum;

			line_stream >> tagname;
			line_stream >> tagnum;

			tagName_.emplace(tagname, tagnum);

			continue;
		}

		if (key == "S") {
			if (object == nullptr) object = std::make_unique<UIObject>();
			UISprite* uiSprite = object->GetComponent<UISprite>();
			if (uiSprite == nullptr) uiSprite = object->AddComponent<UISprite>();

			std::string spritename;
			line_stream >> spritename;

			std::string texname;
			line_stream >> texname;

			Sprite sprite;
			if (texname.find("/") != std::string::npos) {
				std::string dirPath = Util::GetDirectoryPath(texname);
				std::string fileName = Util::GetFileName(texname);

				sprite.Initialize(TextureManager::GetInstance()->AsyncLoadTextureGraph(fileName, dirPath));
			}
			else //	pathが含まれていない
			{
				sprite.Initialize(TextureManager::GetInstance()->AsyncLoadTextureGraph(texname));
			}

			MyMath::Vector2D pos;
			MyMath::Vector2D size;
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> size.x;
			line_stream >> size.y;

			sprite.SetPosition(pos);
			sprite.SetSize(size);

			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> size.x;
			line_stream >> size.y;

			sprite.SetTextureLeftTop(pos);
			sprite.SetTextureSize(size);

			line_stream >> pos.x;
			line_stream >> pos.y;

			sprite.SetAnchorPoint(pos);

			uint16_t tag;
			line_stream >> tag;
			sprite.SetTags(tag);

			uiSprite->AddSprite(spritename, sprite);
		}
		if (key == "AnimeTimer") {
			UIAnimationTimer* uiTimer = object->AddComponent<UIAnimationTimer>();
			uiTimer->Initialize();

			object->SetCount(count_.get());
			object->SetStartAnimation(&startAnimation_);
			uint32_t time;

			line_stream >> time;
			uiTimer->SetMaxFrameCount(time);

			line_stream >> time;
			uiTimer->SetStartCount(time);
		}
		if (key == "MoveAnime") {
			UIMoveAnimation* uiMove = object->AddComponent<UIMoveAnimation>();
			uiMove->Initialize();

			MyMath::Vector2D pos;
			line_stream >> pos.x;
			line_stream >> pos.y;
			uiMove->SetStartPos(pos);

			line_stream >> pos.x;
			line_stream >> pos.y;
			uiMove->SetEndPos(pos);
		}
		if (key == "Scaling") {
			UIScaling* uiScaling = object->AddComponent<UIScaling>();
			uiScaling->Initialize();

			MyMath::Vector2D size;
			line_stream >> size.x;
			line_stream >> size.y;
			uiScaling->SetStartSize(size);

			line_stream >> size.x;
			line_stream >> size.y;
			uiScaling->SetEndSize(size);
		}
		if (key == "FadeAnime") {
			UIFadeAnimation* uiFade = object->AddComponent<UIFadeAnimation>();
			uiFade->Initialize();

			float value;
			line_stream >> value;
			uiFade->SetEnd(value);

			line_stream >> value;
			uiFade->SetStart(value);
		}
		if (key == "RotAnime") {
			UIRotation* uiRot = object->AddComponent<UIRotation>();
			float rot;
			line_stream >> rot;
			uiRot->SetRotSpd(rot);
		}
		if (key == "EndData") {
			std::string spritename;
			line_stream >> spritename;
			object->SetParentData(this);
			obj_.emplace(spritename, std::move(object));
		}

		if (key == "Button") {
			if (object == nullptr) object = std::make_unique<UIObject>();
			UIButton* button = object->AddComponent<UIButton>();

			if (buttonMan_ == nullptr) {
				buttonMan_ = std::make_unique<UIButtonManager>();
				buttonMan_->SetSelectButton(button);
			}
			std::string buttonname;
			line_stream >> buttonname;

			button->SetName(buttonname);
			buttons.emplace(buttonname, button);

			MyMath::Vector2D pos;
			line_stream >> pos.x;
			line_stream >> pos.y;

			UIPosition* uiPos = object->AddComponent<UIPosition>();
			uiPos->SetPosition(pos);

			MyMath::Vector2D size;
			line_stream >> size.x;
			line_stream >> size.y;
			uiPos->SetSize(size);

			line_stream >> size.x;
			line_stream >> size.y;
			size /= 2.0f;
			ButtonSquare coll;
			coll.parent_ = button;
			coll.leftTop_ = pos - size;
			coll.rightBottom_ = pos + size;

			buttonColliders_.push_back(coll);
		}

		if (key == "Slider") {
			SliderSprite* slider = object->AddComponent<SliderSprite>();

			int16_t tag;
			line_stream >> tag;

			std::string name;
			line_stream >> name;
			slider->SetCircleTexture(name, tag);

			line_stream >> name;
			slider->SetRailTexture(name, tag);

			MyMath::Vector2D pos;
			line_stream >> pos.x;
			line_stream >> pos.y;
			slider->SetRailStartPos(pos);

			float len;
			line_stream >> len;
			slider->SetRailLength(len);

			sliderColliders_.push_back(slider);
		}

		if (key == "ButtonArray") {
			std::string buttonname;
			line_stream >> buttonname;

			//	データがなかったら
			if (buttons.count(buttonname) == 0) return;

			std::string nextname;
			line_stream >> nextname;
			std::string prevname;
			line_stream >> prevname;

			if (buttons.count(nextname) != 0) {
				buttons[buttonname]->SetNextButton(buttons[nextname]);
			}
			if (buttons.count(prevname) != 0) {
				buttons[buttonname]->SetPrevButton(buttons[prevname]);
			}
		}
	}

	//ファイル閉じる
	file.close();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

const std::string& MNE::UIData::GetSelectName()
{
	return buttonMan_->GetSelectObjName();
}

MyMath::Vector2D& MNE::UIData::GetSelectPosition()
{
	return buttonMan_->GetSelectPos();
}

MyMath::Vector2D& MNE::UIData::GetSelectSize()
{
	return buttonMan_->GetSelectSize();
}

MNE::UIObject* MNE::UIData::GetUIObject(const std::string& name)
{
	if (obj_.count(name) == 0) return nullptr;

	return obj_[name].get();
}

bool MNE::UIData::GetIsEndAnimation()
{
	//	アニメーションがそもそもなかったら
	if (count_ == nullptr) return true;

	bool isEnd = count_->GetIsActive() == false;
	return isEnd;
}

//-----------------------------------------------------------------------------
// [SECTION] Setter
//-----------------------------------------------------------------------------

void MNE::UIData::Reset()
{
	if (count_ != nullptr) {
		count_->ResetCount();
	}

	for (auto& sprite : obj_) {
		sprite.second->ResetAnimation();
	}
}

bool MNE::UIData::GetSelectMouse()
{
	return InputManager::GetInstance()->GetMouse()->GetClickTrigger(InputMouse::LeftClick) && selecting_;
}

bool MNE::UIData::GetSelecting()
{
	return selecting_;
}

void MNE::UIData::ResetAnimation(bool startingAnimation)
{
	startAnimation_ = startingAnimation;
	
	Reset();

	if (count_ != nullptr) {
		count_->StartCount();
	}
}

void MNE::UIData::SetSelectButton(const std::string& name)
{
	buttonMan_->SetSelectButton(obj_[name]->GetComponent<UIButton>());
}
