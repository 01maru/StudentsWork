#pragma once

/**
* @file IScene.h
* @brief 継承シーンファイル
*/

class IScene
{
public:
	virtual ~IScene() = default;

	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	virtual void Initialize() = 0;

	/**
	* @fn Finalize()
	* exe終了時に呼び出す関数
	*/
	virtual void Finalize() = 0;

	/**
	* @fn LoadResources()
	* リソース読み込み処理関数
	*/
	virtual void LoadResources() = 0;

	/**
	* @fn FirstFrameUpdate()
	* BGM再生などシーンの始まりに呼びたい処理をまとめる関数
	*/
	virtual void FirstFrameUpdate() = 0;

	/**
	* @fn Update()
	* 更新処理関数
	*/
	virtual void Update() = 0;

	/**
	* @fn MatUpdate()
	* 行列更新用関数
	*/
	virtual void MatUpdate() = 0;

	/**
	* @fn ImguiUpdate()
	* ImGui更新処理関数
	*/
	virtual void ImguiUpdate() = 0;

	/**
	* @fn DrawShadow()
	* 影描画用関数
	*/
	virtual void DrawShadow() {};

	/**
	* @fn Draw()
	* 描画処理関数
	*/
	virtual void Draw() = 0;
};

