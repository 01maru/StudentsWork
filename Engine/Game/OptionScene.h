#pragma once
#include "SliderSprite.h"
#include "UIDrawer.h"

/**
* @file OptionScene.h
* @brief Optionの表示や設定を管理するファイル
*/

class OptionScene :public UIDrawer
{
public:
	enum Mord
	{
		Sens,
		Sound,
		Close,
	};
private:
	bool active_ = false;

	uint16_t mord_;

	//	音量
	//	光度
public:
	/**
	* @fn Initialize(const std::string&)
	* 初期化関数
	*/
	void Initialize(const std::string& filename);
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw();

#pragma region Getter

	bool GetIsActive() { return active_; }
	uint16_t GetMord() { return mord_; }

#pragma endregion

	//	Setter
	void SetIsActive(bool active) { active_ = active; }
};

