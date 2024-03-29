#pragma once
#include "GPipeline.h"
#include <vector>
#include <memory>
#include <string>
#include <map>

/**
* @file PipelineManager.h
* @brief 使用するパイプラインをまとめて管理しているファイル
*/

namespace MNE
{

	class PipelineManager
	{
	private:
		std::map<std::string, std::unique_ptr<GPipeline>, std::less<>> pipelines_;

#pragma region Sprite

		std::vector<std::unique_ptr<GPipeline>> spritePipeline_;
		std::unique_ptr<GPipeline> loadingSpritePipe_;		//	Loading用
		std::unique_ptr<GPipeline> dissolveSpritePipe_;		//	シーン遷移用

#pragma endregion

#pragma region Model

		std::unique_ptr<GPipeline> modelSilhouettePipe_;		//	Obj3D単色シェーダー
		std::vector<std::unique_ptr<GPipeline>> modelPipeline_;
		std::unique_ptr<GPipeline> shadowPipeline_;
		std::unique_ptr<GPipeline> shadowRecieverPipeline_;

#pragma endregion

#pragma region Particle

		std::vector<std::unique_ptr<GPipeline>> particlePipeline_;

#pragma endregion

#pragma region PostEffect

		std::unique_ptr<GPipeline> postEffectPipeline_;
		std::unique_ptr<GPipeline> postShadowPipeline_;
		std::unique_ptr<GPipeline> luminncePipeline_;
		std::unique_ptr<GPipeline> xBlurPipeline_;
		std::unique_ptr<GPipeline> yBlurPipeline_;
		std::unique_ptr<GPipeline> luminncexBlurPipeline_;
		std::unique_ptr<GPipeline> luminnceyBlurPipeline_;
		std::unique_ptr<GPipeline> glayScalePipeline_;
		std::unique_ptr<GPipeline> dofPipeline_;

#pragma endregion

	private:
		void InitializeSprite();
		void InitializeParticle();
		void InitializePostEffect();
		void InitializeModel();

		PipelineManager() {};
		~PipelineManager() {};
	public:
		static PipelineManager* GetInstance();
		//	コピーコンストラクタ無効
		PipelineManager(const PipelineManager& obj) = delete;
		//	代入演算子無効
		PipelineManager& operator=(const PipelineManager& obj) = delete;

		/**
		* @fn Initialize()
		* 初期化用関数
		*/
		void Initialize();

#pragma region Getter

		/**
		* @fn GetPipeline(const std::string&, GPipeline::BlendMord)
		* 引数で指定したパイプラインを返す関数
		* @param name パイプラインの名前
		* @param blend パイプラインのブレンドの種類指定
		* @return 引数で指定したパイプラインを返す
		*/
		GPipeline* GetPipeline(const std::string& name, Blend::BlendMord blend = Blend::NONE_BLEND);

#pragma endregion
	};

}
