#pragma once
#include "Sprite.h"

/**
* @file PlayerSkill.h
* @brief スキルのUIを表示するためのファイル
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
	bool isActive_ = FALSE;

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
	* UIのスプライト設定用関数
	* @param sprite スキルのスプライト表示情報
	* @param text 入力方法表示情報
	*/
	virtual void SetSprite(const MNE::Sprite& sprite, const MNE::Sprite& text);

#pragma endregion
};

