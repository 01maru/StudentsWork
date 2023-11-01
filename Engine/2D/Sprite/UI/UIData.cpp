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

void UIData::LoadData(const std::string& filename)
{
	std::string filePath = "Resources/Levels/" + filename + ".txt";

	//ファイル開く(開けなかったら新規作成)
	std::ifstream file;
	file.open(filePath.c_str());

	std::unique_ptr<UIObject> object;
	std::map<std::string, UIButton*, std::less<>> buttons;

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

			Vector2D pos;
			Vector2D size;
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
			uint32_t time;

			line_stream >> time;
			uiTimer->SetMaxFrameCount(time);

			line_stream >> time;
			uiTimer->SetStartCount(time);
		}
		if (key == "MoveAnime") {
			UIMoveAnimation* uiMove = object->AddComponent<UIMoveAnimation>();
			uiMove->Initialize();

			Vector2D pos;
			line_stream >> pos.x;
			line_stream >> pos.y;
			uiMove->SetStartPos(pos);

			line_stream >> pos.x;
			line_stream >> pos.y;
			uiMove->SetEndPos(pos);
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
			obj_.emplace(spritename, std::move(object));
		}

		if (key == "Button") {
			UIButton* button = object->AddComponent<UIButton>();

			if (buttonMan_ == nullptr) {
				buttonMan_ = std::make_unique<UIButtonManager>();
				buttonMan_->SetSelectButton(button);
			}
			std::string buttonname;
			line_stream >> buttonname;

			button->SetName(buttonname);
			buttons.emplace(buttonname, button);

			Vector2D pos;
			line_stream >> pos.x;
			line_stream >> pos.y;

			UIPosition* uiPos = object->AddComponent<UIPosition>();
			uiPos->SetPosition(pos);
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

void UIData::Initialize()
{
	count_->StartCount();
}

void UIData::Finalize()
{
	count_.release();
	obj_.clear();
	tagName_.clear();
}

void UIData::InputUpdate()
{
	buttonMan_->Update();

}

void UIData::Update()
{
	for (auto& sprite : obj_) {
		sprite.second->Update();
		sprite.second->MatUpdate();
	}

	//	アニメーション
		//	アニメーションタイマーがあるか検索
			//	あったら比較してカウントアクティブにする
	if (count_ != nullptr) {
		count_->Update();
	}

	//buttonMan_.MatUpdate();
}

void UIData::Draw()
{
	for (auto& sprite : obj_) {
		//if (sprite.second.GetTags() & data_->tagName_[activeTagName_]) {
		sprite.second->Draw();
		//}
	}

	//buttonMan_.Draw();
}

//-----------------------------------------------------------------------------
// [SECTION] Getter
//-----------------------------------------------------------------------------

const std::string& UIData::GetSelectName()
{
	return buttonMan_->GetSelectObjName();
}

Vector2D& UIData::GetSelectPosition()
{
	return buttonMan_->GetSelectPos();
}

UIObject* UIData::GetUIObject(const std::string& name)
{
	if (obj_.count(name) == 0) return nullptr;

	return obj_[name].get();
}
