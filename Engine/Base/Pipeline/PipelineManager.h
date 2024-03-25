#pragma once
#include "GPipeline.h"
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
		/**
		* @fn ImGuiUpdate()
		* ImGui更新用関数
		*/
		void ImGuiUpdate();

	private:
		//	パイプラインのマップ
		std::map<std::string, std::unique_ptr<GPipeline>, std::less<>> pipelines_;

	private:
		void InitializeSprite();
		void InitializeParticle();
		void InitializePostEffect();
		void InitializeModel();

	public:
		bool FindPipeline(const std::string& name);

		GPipeline* AddPipeline(std::unique_ptr<GPipeline>& pipeline, const std::string& name);
		
		GPipeline* AddPipeline(std::unique_ptr<GPipeline>& pipeline, const std::string& name, int32_t blend);

		void DeletePipeline(const std::string& name);

#pragma region Getter

		/**
		* @fn GetPipeline(const std::string&)
		* 引数で指定したパイプラインを返す関数
		* @param name パイプラインの名前
		* @return 引数で指定したパイプラインを返す
		*/
		GPipeline* GetPipeline(const std::string& name);
		/**
		* @fn GetPipeline(const std::string&, GPipeline::BlendMode)
		* 引数で指定したパイプラインを返す関数
		* @param name パイプラインの名前
		* @param blend パイプラインのブレンドの種類指定
		* @return 引数で指定したパイプラインを返す
		*/
		GPipeline* GetPipeline(const std::string& name, Blend::BlendMode blend);

#pragma endregion
	};

}
