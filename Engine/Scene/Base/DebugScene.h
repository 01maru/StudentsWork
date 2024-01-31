#pragma once
#include "IScene.h"
#include "Object3D.h"
#include <memory>

#include "EscapePod.h"

#include "FrameCounter.h"

/**
* @file DebugScene.h
* @brief デバッグで確認する用のシーンファイル
*/

namespace MNE
{

	class DebugScene :public IScene
	{
	private:
		std::unique_ptr<Object3D> skydome_;
		std::unique_ptr<Object3D> ground_;
		int32_t index = -1;
		int32_t timer = 0;

		std::unique_ptr<EscapePod> ship_;

	private:	//	関数
		/**
		* @fn MatUpdate()
		* 行列更新用関数
		*/
		void MatUpdate() override;

	public:
		~DebugScene() override {};

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
		void DrawShadow() override;

		/**
		* @fn Draw()
		* 描画処理関数
		*/
		void Draw() override;
	};

}
