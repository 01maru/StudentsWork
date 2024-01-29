#pragma once
#include "FrameCounter.h"
#include "PlayerSkill.h"

/**
* @file CoolTime.h
* @brief スキルのクールタイムを管理するファイル
*/

class SkillCoolTime :public PlayerSkill
{
public:
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize() override;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw() override;

protected:
	//	クールタイムのタイマー
	FrameCounter coolTimer_;

#pragma region Sprite

	//	ゲージ表示用
	MNE::Sprite gauge_;

#pragma endregion

private:
	/**
	* @fn GaugeUpdate()
	* ゲージの更新処理関数
	*/
	void GaugeUpdate();

public:
#pragma region Setter

	void StartCount();
	void SetSprite(const MNE::Sprite& sprite, const MNE::Sprite& text) override;
	void SetMaxTime(int32_t time);

#pragma endregion
};

