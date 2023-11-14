#pragma once
#include "SliderSprite.h"
#include "UIData.h"
#include "XAudioManager.h"
#include "SelectCursor.h"

/**
* @file OptionScene.h
* @brief Optionの表示や設定を管理するファイル
*/

class OptionScene
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize();
	/**
	* @fn LoadResources(const std::string&)
	* リソース読み込み用関数
	* @param filename レベルデータのファイル名
	*/
	void LoadResources(const std::string& filename);
	void InputUpdate(bool selectButton);
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();
	/**
	* @fn ImGuiUpdate()
	* ImGui更新処理関数
	*/
	void ImGuiUpdate();
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw();

	void ResetSelectButton();

private:
	bool isActive_ = false;
	
	UIData data_;

	SelectCursor* cursor_ = nullptr;

	Vector2D backPos_;

	//	光度
private:
	/**
	* @fn SensUpdate()
	* 視点感度更新処理用関数
	*/
	void SensUpdate(int16_t inputValue);
	/**
	* @fn VolumeUpdate()
	* ボリューム更新処理用関数
	*/
	void VolumeUpdate(const std::string& objectName, XAudioManager::SoundType type, int16_t inputValue);

public:
#pragma region Getter

	bool GetIsActive();
	Vector2D& GetSelectPosition();
	Vector2D GetSelectScale();

#pragma endregion

#pragma region Setter

	void SetCursorBackPos(const Vector2D& pos);
	void SetSelectCursor(SelectCursor* cursor);
	void SetIsActive(bool active);

#pragma endregion
};

