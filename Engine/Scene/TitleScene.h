#pragma once
#include "IScene.h"
#include "TitleUI.h"
#include "Bonfire.h"

/**
* @file TitleScene.h
* @brief タイトルシーンファイル
*/

class TitleScene :public MNE::IScene
{
public:
	//	デストラクタ
	~TitleScene() override {};

	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize() override;
	/**
	* @fn Finalize()
	* exe終了時に呼び出す関数
	*/
	void Finalize() override;
	/**
	* @fn LoadResources()
	* リソース読み込み処理関数
	*/
	void LoadResources() override;
	/**
	* @fn FirstFrameUpdate()
	* BGM再生などシーンの始まりに呼びたい処理をまとめる関数
	*/
	void FirstFrameUpdate() override;
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;
	/**
	* @fn MatUpdate()
	* 行列更新用関数
	*/
	void MatUpdate() override;
	/**
	* @fn ImguiUpdate()
	* ImGui更新処理関数
	*/
	void ImguiUpdate() override;
	void DrawUIBeforeBlackScreen() override;
	void DrawUIAfterBlackScreen() override;
	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw() override;

private:
	std::vector<std::unique_ptr<MNE::Object3D>> objs_;

	std::unique_ptr<Bonfire> bonfire_;

#pragma region Fog

	MyMath::Vector3D fogColor_ = { 1.0f,1.0f,1.0f };
	float fogStart_ = 2.2f;
	float fogEnd_ = 6.0f;
	float fogFar_ = 30.0f;

#pragma endregion

#pragma region Sprite

	TitleUI uiData_;
	bool drawUI_ = true;

#pragma endregion
};

