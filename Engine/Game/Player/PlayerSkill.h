#pragma once
#include "Sprite.h"

/**
* @file CoolTime.h
* @brief スキルのクールタイムを管理するファイル
*/

class PlayerSkill
{
public:
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	virtual void Initialize();
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	virtual void Draw();

protected:
	//	スキルが使えるか
	bool isActive_ = false;

#pragma region Sprite

	//	スキルの内容
	MNE::Sprite sprite_;
	//	入力キー表示
	MNE::Sprite text_;

#pragma endregion

public:
#pragma region Getter

	/**
	* @fn GetIsActive()
	* スキル使用可能か取得関数
	* @return スキル使用可能か
	*/
	bool GetIsActive();

#pragma endregion

#pragma region Setter

	/**
	* @fn SetSprite(const MNE::Sprite&, const MNE::Sprite&)
	* カメラの移動時間設定用関数
	* @param sprite 選択中かどうか
	* @param text 選択中かどうか
	*/
	virtual void SetSprite(const MNE::Sprite& sprite, const MNE::Sprite& text);

#pragma endregion
};

