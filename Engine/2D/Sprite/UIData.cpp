#include "UIData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "TextureManager.h"
#include "ConvertString.h"
#include "UIAnimationTimer.h"
#include "UIMoveAnimation.h"

void UIData::LoadData(const std::string& filename)
{
	buttonMan_.Initialize();
	std::string filePath = "Resources/Levels/" + filename + ".txt";

	//ファイル開く(開けなかったら新規作成)
	std::ifstream file;
	file.open(filePath.c_str());

	//if (file.fail()) {
	//	if (editUI_) return ans;
	//}

	std::unique_ptr<UIObject> object;
	UISprite* uiSprite = nullptr;

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
			//	本当は最初だけ
			if (object == nullptr) object = std::make_unique<UIObject>();
			uiSprite = object->GetComponent<UISprite>();
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
			//data_->sprites_.emplace(spritename, sprite);
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
		if (key == "EndData") {
			std::string spritename;
			line_stream >> spritename;
			obj_.emplace(spritename, std::move(object));
		}

		if (key == "B") {
			std::string buttonname;
			line_stream >> buttonname;

			std::string texname;
			line_stream >> texname;

			UIButton button;
			if (texname.find("/") != std::string::npos) {
				std::string dirPath = Util::GetDirectoryPath(texname);
				std::string fileName = Util::GetFileName(texname);

				button.Initialize(TextureManager::GetInstance()->AsyncLoadTextureGraph(fileName, dirPath));
			}
			else //	pathが含まれていない
			{
				button.Initialize(TextureManager::GetInstance()->AsyncLoadTextureGraph(texname));
			}

			Vector2D pos;
			Vector2D size;
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> size.x;
			line_stream >> size.y;

			button.SetPosition(pos);
			button.SetSize(size);

			button.SetAnchorPoint({ 0.5f,0.5f });

			uint16_t tag;
			line_stream >> tag;
			button.SetTags(tag);

			buttonMan_.LoadUIButton(button, buttonname);
		}
	}

	//ファイル閉じる
	file.close();
}

void UIData::Finalize()
{
	count_.release();
	obj_.clear();
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

	buttonMan_.MatUpdate();
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
