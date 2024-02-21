#pragma once
#include "FrameCounter.h"
#include "PlayerSkill.h"
#include "SkillActiveEffect.h"

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
	MNE::FrameCounter coolTimer_;

	float rotMax_ = 0.1f;
	float shakeMax_ = 5.0f;
	int32_t shakeTime_ = 5;
	MNE::FrameCounter shakeTimer_;

	MyMath::Vector2D centerPos_;

	SkillActiveEffect effect_;

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
	/**
	* @fn ShakeUpdate()
	* 使用不可時に入力した際のアニメーション処理関数
	*/
	void ShakeUpdate();

public:
#pragma region Setter

	/**
	* @fn StartShakeAnime()
	* シェイクアニメーション開始用関数
	*/
	void StartShakeAnime();
	/**
	* @fn StartCount()
	* スキルのクールタイム開始用関数
	*/
	void StartCount();
	/**
	* @fn SetSprite(const MNE::Sprite&, const MNE::Sprite&, MNE::Texture*)
	* UIのスプライト設定用関数
	* @param sprite スキルのスプライト表示情報
	* @param text 入力方法表示情報
	* @param padTex パッドの入力方法表示用テクスチャ
	*/
	void SetSprite(const MNE::Sprite& sprite, const MNE::Sprite& text, MNE::Texture* padTex) override;
	/**
	* @fn SetMaxTime(int32_t)
	* スキルのクールタイム総時間設定用関数
	* @param time スキルのクールタイム総時間
	*/
	void SetMaxTime(int32_t time);

#pragma endregion
};

