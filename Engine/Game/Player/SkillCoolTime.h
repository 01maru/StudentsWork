#pragma once
#include "FrameCounter.h"
#include "PlayerSkill.h"

/**
* @file SkillCoolTime.h
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

	/**
	* @fn StartCount()
	* スキルのクールタイム開始用関数
	*/
	void StartCount();
	/**
	* @fn SetSprite(const MNE::Sprite&, const MNE::Sprite&)
	* UIとして表示するために必要なスプライト設定用関数
	* @param sprite スキルの見た目がわかるスプライト
	* @param text スキルを使用するための入力方法表記用スプライト
	*/
	void SetSprite(const MNE::Sprite& sprite, const MNE::Sprite& text) override;
	/**
	* @fn SetMaxTime(int32_t)
	* スキルのクールタイム総時間設定用関数
	* @param time スキルのクールタイム総時間
	*/
	void SetMaxTime(int32_t time);

#pragma endregion
};

