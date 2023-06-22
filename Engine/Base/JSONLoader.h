#pragma once
#include <string>
#include "Vector3D.h"
#include "IModel.h"
#include "Object3D.h"
#include <json.hpp>

struct ObjectData
{
	//	ファイル名
	std::string fileName;
	//	オブジェクト情報
	Vector3D translation;
	Vector3D rotation;
	Vector3D scaling;
	//	親情報
	ObjectData* parent = nullptr;
};

struct CameraData
{
	Vector3D eye;
	Vector3D target;
};

struct LevelData
{
	std::vector<ObjectData> objects;
};

class JSONLoader
{
private:
	//	モデルデータコンテナ
	std::map<std::string, std::unique_ptr<IModel>> models_;
	//	オブジェクトデータ
	std::vector<std::unique_ptr<Object3D>> objects_;
	std::unique_ptr<LevelData> levelData_;

	CameraData cameraData;

	void LoadObjectData(nlohmann::json_abi_v3_11_2::detail::iter_impl<nlohmann::json_abi_v3_11_2::json>& itr, ObjectData* parent);
public:
	void LoadJSON(std::string jsonname);
	void LoadModel();
	void MatUpdate();
	void Draw();
};

