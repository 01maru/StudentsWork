#pragma once
#include "IScene.h"
#include "ILoadingObj.h"
#include "DissolveSprite.h"
#include "FrameCounter.h"
#include "LoadSpriteAnime.h"
#include <memory>

class LoadingScene :public MNE::IScene
{
public:
	/**
	* @fn Initialize()
	* 初期化用関数
	*/
	void Initialize() override;
	/**
	* @fn Finalize()
	* exe終了時に呼び出す関数
	*/
	void Finalize() {};
	/**
	* @fn LoadResources()
	* リソース読み込み処理関数
	*/
	void LoadResources();
	/**
	* @fn FirstFrameUpdate()
	* BGM再生などシーンの始まりに呼びたい処理をまとめる関数
	*/
	void FirstFrameUpdate() {};

	/**
	* @fn MatUpdate()
	* 行列更新用関数
	*/
	void MatUpdate() {};
	/**
	* @fn ImguiUpdate()
	* ImGui更新処理関数
	*/
	void ImguiUpdate() {};
	/**
	* @fn Update()
	* 更新処理関数
	*/
	void Update() override;

	/**
	* @fn Draw()
	* 描画処理関数
	*/
	void Draw() override;
	
private:
	//std::unique_ptr<ILoadingObj> loadObj_;
	std::unique_ptr<LoadSpriteAnime> loadSprite_;
	//	ロード画面の背景
	std::unique_ptr<MNE::DissolveSprite> backSprite_;
	bool isDraw_ = false;
	//	フェードインアウト用カウンター
	FrameCounter counter_;
	const int32_t MAX_COUNT = 60;
	const int32_t OBJ_MAX_COUNT = 30;
	int32_t backEasePow_ = 2;
	Vector2D texSize_;

public:
	void SetIsLoading(bool loading);

	bool GetIsDrawn();

	void StartFadeAnimation(bool isFadeIn);
};

