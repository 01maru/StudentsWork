#pragma once
#include "AbstractSceneFactory.h"

/**
* @file SplashScreenScene.h
* @brief ファイル
*/

namespace MNE
{

	class SceneFactory :public AbstractSceneFactory
	{
	public:
		/**
		* @fn CreateScene(const std::string&)
		* シーンを生成する用関数
		* @param sceneName シーンの名前
		* @return 生成するシーン
		*/
		std::unique_ptr<IScene> CreateScene(const std::string& sceneName) override;
	};

}
