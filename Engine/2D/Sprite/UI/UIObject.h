#pragma once
#include <list>
#include <memory>
#include "UIComponent.h"

/**
* @file UIObject.h
* @brief コンポーネント指向で作成したUI用Objectファイル
*/

#pragma region 前置宣言

class FrameCounter;

#pragma endregion


class UIObject
{
public:
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

private:
	//	コンポーネントリスト
	std::list<std::unique_ptr<UIComponent>> components_;
	//	全体のcounterのポインター(実体はunique_ptrなのでdeleteいらない)
	FrameCounter* count_ = nullptr;
	bool* startAnimation_ = nullptr;

public:

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
	/**
	* @fn GetCount()
	* 全体のcounterのポインターのGetter関数
	* @return 全体のcounterのポインター
	*/
	FrameCounter* GetCount();

	bool GetIsStartAnimation();

#pragma endregion

#pragma region Setter

	/**
	* @fn SetCount(FrameCounter*)
	* 全体のcounterのポインターをセットするための関数
	* @param pCount 全体のcounterのポインター
	*/
	void SetCount(FrameCounter* pCount);
	void SetStartAnimation(bool* pStartAnimation);

	void ResetAnimation();

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
