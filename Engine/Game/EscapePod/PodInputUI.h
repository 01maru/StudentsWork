#pragma once
#include "Sprite.h"
#include "FrameCounter.h"

/**
* @file PodInputUI.h
* @brief 脱出ポッドの入力説明用スプライトの表示を管理するファイル
*/

class PodInputUI
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize();
	/**
	* @fn LoadResources()
	* リソース読み込み処理関数
	*/
	void LoadResources();
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

private:
	//	描画中だったら
	bool isActive_ = FALSE;
	//	フェードインアウトタイマー
	MNE::FrameCounter counter_;
	int32_t fadeMaxTime_ = 30;
	//	UIのスプライト
	MNE::Sprite ui_;
	MNE::Sprite button_;
	//	入力用画像
	MNE::Texture* padTex_ = nullptr;
	MNE::Texture* keyTex_ = nullptr;

private:
	/**
	* @fn ChangeSpriteTex()
	* 入力に応じたスプライトの変更処理関数
	*/
	void ChangeSpriteTex();

public:
#pragma region Setter

	/**
	* @fn Start()
	* 表示するアニメーション開始用関数
	*/
	void Start();
	/**
	* @fn SetSprite(const MNE::Sprite&, const MNE::Sprite&)
	* スプライトの情報設定用関数
	* @param uiSprite テキストスプライトの配置情報
	* @param buttonSprite 入力スプライトの配置情報
	*/
	void SetSprite(const MNE::Sprite& uiSprite, const MNE::Sprite& buttonSprite);
	/**
	* @fn SetIsAcitve(bool)
	* 実行中かのフラグ設定用関数
	* @param isActive 実行中かどうか
	*/
	void SetIsAcitve(bool isActive);

#pragma endregion

};

