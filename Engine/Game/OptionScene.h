#pragma once
#include "SliderSprite.h"
#include "UIData.h"

/**
* @file OptionScene.h
* @brief Optionの表示や設定を管理するファイル
*/

class OptionScene
{
public:
	/**
	* @fn Initialize(const std::string&)
	* 初期化関数
	* @param filename レベルデータのファイル名
	*/
	void Initialize(const std::string& filename);
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

	//	音量
	//	光度
private:
	/**
	* @fn SensUpdate()
	* 視点感度更新処理用関数
	*/
	void SensUpdate();
	/**
	* @fn MasterVolumeUpdate()
	* マスターボリューム更新処理用関数
	*/
	void MasterVolumeUpdate();
	/**
	* @fn BGMVolumeUpdate()
	* BGM音量更新処理用関数
	*/
	void BGMVolumeUpdate();
	/**
	* @fn UpdSEVolumeUpdateate()
	* SE音量更新処理関数
	*/
	void SEVolumeUpdate();

public:
#pragma region Getter

	bool GetIsActive();

#pragma endregion

#pragma region Setter

	void SetIsActive(bool active);

#pragma endregion
};

