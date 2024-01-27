#include "JSONLoader.h"
#include <fstream>
#include <cassert>
#include "MyMath.h"
#include "ModelManager.h"

using namespace std;
using namespace MNE;

void MNE::JSONLoader::LoadCameraData(JSONData& data, nlohmann::json_abi_v3_11_2::detail::iter_impl<nlohmann::json_abi_v3_11_2::json>& itr)
{
	string tag = itr.value()["Tag"];
	if (data.GetCameraDatum().count(tag) == 0)
	{
		data.GetCameraDatum().emplace(tag, CameraData());
	}

	data.GetCameraDatum()[tag].targetID = itr.value()["targetID"];

	//	トランスフォームのパラメータ読み込み
	nlohmann::json& transform = itr.value()["transform"];

	//	平行移動
	data.GetCameraDatum()[tag].eye.x = (float)transform["translation"][1];
	data.GetCameraDatum()[tag].eye.y = (float)transform["translation"][2];
	data.GetCameraDatum()[tag].eye.z = -(float)transform["translation"][0];
}

void MNE::JSONLoader::LoadEmpty(JSONData& data, nlohmann::json_abi_v3_11_2::detail::iter_impl<nlohmann::json_abi_v3_11_2::json>& itr)
{
	if (itr.value().contains("name")) {
		std::string name = itr.value()["name"];
		if (name.find("Target") != std::string::npos) {
			int32_t id = itr.value()["ID"];
			string tagName = "";
			for (auto camera : data.GetCameraDatum())
			{
				if (camera.second.targetID == id)
				{
					tagName = camera.first;
				}
			}
			//	トランスフォームのパラメータ読み込み
			nlohmann::json& transform = itr.value()["transform"];
			//	平行移動
			data.GetCameraDatum()[tagName].target.x = (float)transform["translation"][1];
			data.GetCameraDatum()[tagName].target.y = (float)transform["translation"][2];
			data.GetCameraDatum()[tagName].target.z = -(float)transform["translation"][0];
		}
		else if (itr.value()["name"] == "PlayerSpawn") {
			//	トランスフォームのパラメータ読み込み
			nlohmann::json& transform = itr.value()["transform"];
			data.GetSpawnData().emplace_back(SpawnPoint());
			SpawnPoint* spawner = &data.GetSpawnData().back();
			//	平行移動
			spawner->pos.x = (float)transform["translation"][1];
			spawner->pos.y = (float)transform["translation"][2];
			spawner->pos.z = -(float)transform["translation"][0];
			//	回転角
			spawner->rotation.x = -(float)transform["rotation"][1];
			spawner->rotation.y = -(float)transform["rotation"][2];
			spawner->rotation.z = (float)transform["rotation"][0];
		}
	}
}

void JSONLoader::LoadObjectData(JSONData& data, nlohmann::json_abi_v3_11_2::detail::iter_impl<nlohmann::json_abi_v3_11_2::json>& itr, ObjectData* parent)
{
	//	種別取得
	string type = itr.value()["type"].get<string>();

	ObjectData* objectData = nullptr;
	//	種類ごとに処理

	//	MESHだったら
	if (type.compare("MESH") == 0) {
		data.GetObjectData().emplace_back(ObjectData{});
		objectData = &data.GetObjectData().back();

		if (itr.value().contains("file_name")) {
			//	ファイル名
			objectData->fileName = itr.value()["file_name"];

			//	fbxかの判定まだ
			ModelManager::GetInstance()->LoadModel(objectData->fileName);
		}

		//	トランスフォームのパラメータ読み込み
		nlohmann::json& transform = itr.value()["transform"];
		//	平行移動
		objectData->translation.x = (float)transform["translation"][1];
		objectData->translation.y = (float)transform["translation"][2];
		objectData->translation.z = -(float)transform["translation"][0];
		//	回転角
		objectData->rotation.x = -MyMath::ConvertToRad((float)transform["rotation"][1]);
		objectData->rotation.y = -MyMath::ConvertToRad((float)transform["rotation"][2]);
		objectData->rotation.z = MyMath::ConvertToRad((float)transform["rotation"][0]);
		//	スケーリング
		objectData->scaling.x = (float)transform["scaling"][1];
		objectData->scaling.y = (float)transform["scaling"][2];
		objectData->scaling.z = (float)transform["scaling"][0];

		objectData->parent = parent;
	}
	
	//	再帰処理
	if (itr.value().contains("children")) {
		for (auto itr_cild = itr.value()["children"].begin(); itr_cild < itr.value()["children"].end(); ++itr_cild)
		{
			LoadObjectData(data, itr_cild, objectData);
		}
	}
}

JSONData MNE::JSONLoader::LoadJSON(const std::string& jsonname)
{
	std::ifstream file;
	const string filename = jsonname + ".json";
	const string directoryPath = "Resources/Levels/";

	//	ファイルを開く
	file.open(directoryPath + filename);

	//	失敗したら
	if (file.fail()) assert(0);

	nlohmann::json deserialized;
	//	JSON文字列から解凍
	file >> deserialized;

	//	正しいデータかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//	"name"を文字列取得
	string name = deserialized["name"].get<string>();
	//	正しいデータかチェック
	assert(name.compare("scene") == 0);

	//	格納する用
	JSONData loadData;

	//	データ読み込み
	for (auto itr = deserialized["objects"].begin(); itr < deserialized["objects"].end(); ++itr)
	{
		assert(itr.value().contains("type"));

		//	種別取得
		string type = itr.value()["type"].get<string>();

		if (type.compare("CAMERA") == 0) {
			LoadCameraData(loadData, itr);
		}
		
		else if (type.compare("EMPTY") == 0) {
			LoadEmpty(loadData, itr);
		}

		else if (type.compare("MESH") == 0) {
			LoadObjectData(loadData ,itr, nullptr);
		}
	}

	return loadData;
}
