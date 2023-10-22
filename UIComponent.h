#pragma once

/**
* @file UIComponent.h
* @brief コンポーネント指向で作成する用のUI用継承用ファイル
*/

class UIObject;

class UIComponent
{
protected:
	UIObject* parent_ = nullptr;

public:
	virtual ~UIComponent() = default;
	/**
	* @fn Initialize()
	* 初期化関数
	*/
	virtual void Initialize() {};
	/**
	* @fn Update()
	* 更新処理関数
	*/
	virtual void Update() {};
	/**
	* @fn MatUpdate()
	* 座標更新処理関数
	*/
	virtual void MatUpdate() {};
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	virtual void Draw() {};

	//	Setter
	/**
	* @fn SetParent(UIObject*)
	* parent_の設定用関数
	* @param parent parent_の変更後の値
	*/
	void SetParent(UIObject* parent);
};

