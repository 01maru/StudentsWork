#pragma once
#include "IScene.h"
#include "SplashSprite.h"
#include "FrameCounter.h"

/**
* @file SplashScreenScene.h
* @brief スプラッシュスクリーン用のシーンファイル
*/

class SplashScreenScene :public IScene
{
private:
	std::unique_ptr<FrameCounter> count_;

	std::unique_ptr<SplashSprite> engineSprite_;
	std::unique_ptr<Sprite> backSprite_;

private:	//	関数
	/**
	* @fn MatUpdate()
	* 行列更新用関数
	*/
	void MatUpdate() override {};

public:
	~SplashScreenScene() override {};

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
	void FirstFrameUpdate() override {};

	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;

	/**
	* @fn ImguiUpdate()
	* ImGui更新処理関数
	*/
	void ImguiUpdate() override;

	/**
	* @fn DrawShadow()
	* 影描画用関数
	*/
	void DrawShadow() override {};

	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw() override;

#pragma region Getter

	/**
	* @fn EndScene()
	* スプラッシュスクリーン終了判定用関数
	* @return スプラッシュスクリーンの処理が終わったらtrueを返す
	*/
	bool EndScene() { return !count_->GetIsActive(); }

#pragma endregion
};

