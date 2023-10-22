#pragma once
#include <list>
#include <memory>
#include "UIComponent.h"
#include "UISprite.h"
#include "FrameCounter.h"

/**
* @file UIObject.h
* @brief コンポーネント指向で作成したUI用Objectファイル
*/

class UIObject
{
private:
	//	コンポーネントリスト
	std::list<std::unique_ptr<UIComponent>> components_;
	
	FrameCounter* count_ = nullptr;

public:
	UIObject() {};

	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update();
	/**
	* @fn MatUpdate()
	* 座標更新処理関数
	*/
	void MatUpdate();
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw();

	/**
	* @fn AddComponent()
	* コンポーネント追加用関数
	*/
	template<class T>
	T* AddComponent();

	/**
	* @fn DeleteComponent()
	* コンポーネント削除用関数
	*/
	template<class T>
	void DeleteComponent();

#pragma region Getter

	/**
	* @fn GetComponent()
	* コンポーネント取得用関数
	*/
	template<class T>
	T* GetComponent();

	FrameCounter* GetCount() { return count_; }

#pragma endregion

#pragma region Setter

	void SetCount(FrameCounter* pCount) { count_ = pCount; }

#pragma endregion
};

template<class T>
inline T* UIObject::AddComponent()
{
	std::unique_ptr<T> component = std::make_unique<T>();
	component->SetParent(this);
	component->Initialize();
	components_.push_back(std::move(component));

	return dynamic_cast<T*>(components_.back().get());
}

template<class T>
inline T* UIObject::GetComponent()
{
	for (auto itr = components_.begin(); itr != components_.end(); ++itr)
	{
		T* compo = dynamic_cast<T*>(itr->get());
		if (compo != nullptr)	return compo;
	}
	return nullptr;
}

template<class T>
void UIObject::DeleteComponent()
{
}
