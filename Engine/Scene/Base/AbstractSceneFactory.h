#pragma once
#include <memory>
#include <string>

/**
* @file SplashScreenScene.h
* @brief ファイル
*/

class IScene;

class AbstractSceneFactory
{
public:
	virtual ~AbstractSceneFactory() = default;
	/**
	* @fn CreateScene(const std::string&)
	* シーンを生成する用関数
	* @param sceneName シーンの名前
	* @return 生成するシーン
	*/
	virtual std::unique_ptr<IScene> CreateScene(const std::string& sceneName) = 0;
};

