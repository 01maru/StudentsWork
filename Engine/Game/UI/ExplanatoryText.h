#pragma once
#include "UIData.h"
#include "Sprite.h"

/**
* @file ExplanatoryText.h
* @brief 操作説明テキストを管理するファイル
*/

#pragma region 前置宣言
namespace MNE
{
	class Texture;
}
#pragma endregion

class ExplanatoryText :public MNE::UIData
{
public:
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	void Initialize();
	/**
	* @fn LoadResources(const std::string&, const std::string&)
	* リソース読み込み用関数
	* @param uiFileName UIDataのファイル名
	* @param cursorSpriteName マウスカーソルのスプライト名
	*/
	void LoadResources(const std::string& uiFileName, const std::string& cursorSpriteName);
	/**
	* @fn Update(bool)
	* 更新処理関数
	* @param prevUsePad 1フレーム前にPadが使用されているか
	*/
	void Update(bool prevUsePad);
	/**
	* @fn MatUpdate()
	* 定数バッファ更新処理関数
	*/
	void MatUpdate();
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw();

private:
	//	実行中か
	bool isActive_ = false;

	//	マウスカーソルスプライト
	MNE::Sprite cursor_;
	//	マウスカーソル画像
	MNE::Texture* cursorTex_ = nullptr;

	//	次のタグ
	std::string nextTag_ = "";

private:
	/**
	* @fn SetNextTagUpdate()
	* 次のタグが設定されていた際のタグ更新関数
	*/
	void SetNextTagUpdate();

public:
#pragma region Setter

	/**
	* @fn SetIsActive(bool)
	* アクティブか設定用関数
	* @param isActive アクティブか
	*/
	void SetIsActive(bool isActive);
	/**
	* @fn SetNextAnimationTag(const std::string&, bool, bool)
	* 次表示するタグ設定用関数
	* @param tagName セットするタグの名前
	* @param playAnimation アニメーションするかどうか
	* @param startingAnimation 開始時アニメーションかどうか
	*/
	void SetNextAnimationTag(const std::string& tagName, bool playAnimation, bool startingAnimation);

#pragma endregion
};

