#include "JSONLoader.h"
#include <fstream>
#include <assert.h>
#include "CameraManager.h"
#include "GameCamera.h"

#include "ObjModel.h"
#include "MyMath.h"

#include <map>

using namespace std;

void JSONLoader::LoadObjectData(nlohmann::json_abi_v3_11_2::detail::iter_impl<nlohmann::json_abi_v3_11_2::json>& itr, ObjectData* parent)
{
	//	種別取得
	string type = itr.value()["type"].get<string>();

	ObjectData* objectData = nullptr;
	//	種類ごとに処理

	//	CAMERAだったら
	if (type.compare("CAMERA") == 0) {

		//	トランスフォームのパラメータ読み込み
		nlohmann::json& transform = itr.value()["transform"];
		//	平行移動
		cameraData_.eye.x = (float)transform["translation"][1];
		cameraData_.eye.y = (float)transform["translation"][2];
		cameraData_.eye.z = -(float)transform["translation"][0];
	}
	if (type.compare("EMPTY") == 0) {

		if (itr.value().contains("name")) {
			if (itr.value()["name"] == "Target") {
				//	トランスフォームのパラメータ読み込み
				nlohmann::json& transform = itr.value()["transform"];
				//	平行移動
				cameraData_.target.x = (float)transform["translation"][1];
				cameraData_.target.y = (float)transform["translation"][2];
				cameraData_.target.z = -(float)transform["translation"][0];
			}
		}
		if (itr.value().contains("name")) {
			if (itr.value()["name"] == "PlayerSpawn") {
				//	トランスフォームのパラメータ読み込み
				nlohmann::json& transform = itr.value()["transform"];
				//	平行移動
				playerData_.pos.x = (float)transform["translation"][1];
				playerData_.pos.y = (float)transform["translation"][2];
				playerData_.pos.z = -(float)transform["translation"][0];
				//	回転角
				playerData_.rotation.x = -(float)transform["rotation"][1];
				playerData_.rotation.y = -(float)transform["rotation"][2];
				playerData_.rotation.z = (float)transform["rotation"][0];
			}
		}
	}
	//	MESHだったら
	if (type.compare("MESH") == 0) {
		levelData_->objects.emplace_back(ObjectData{});
		objectData = &levelData_->objects.back();

		if (itr.value().contains("file_name")) {
			//	ファイル名
			objectData->fileName = itr.value()["file_name"];
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
			LoadObjectData(itr_cild, objectData);
		}
	}
}

void JSONLoader::LoadJSON(const std::string& jsonname)
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

	//	格納用
	levelData_ = std::make_unique<LevelData>();

	// "objects"の全オブジェクト走査
	for (auto itr = deserialized["objects"].begin(); itr < deserialized["objects"].end(); ++itr)
	{
		assert(itr.value().contains("type"));

		LoadObjectData(itr, nullptr);
	}

	LoadModel();

	//std::unique_ptr<GameCamera> camera = std::make_unique<GameCamera>();
	//camera->Initialize(cameraData_.eye, cameraData_.target, Vector3D(0.0f, 1.0f, 0.0f));
	//CameraManager::GetInstance()->SetMainCamera(std::move(camera));

	for (auto& objectData : levelData_->objects)
	{
		IModel* model = nullptr;
		decltype(models_)::iterator it = models_.find(objectData.fileName);

		if (it != models_.end()) { model = it->second.get(); }

		std::unique_ptr<Object3D> newObject;
		newObject.reset(Object3D::Create(model));
		
		newObject->SetPosition(objectData.translation);
		newObject->SetRotation(objectData.rotation);
		newObject->SetScale(objectData.scaling);
		
		objects_.push_back(std::move(newObject));
	}
}

void JSONLoader::LoadModel()
{
	for (auto& objectData : levelData_->objects) {
		decltype(models_)::iterator it = models_.find(objectData.fileName);
		if (it == models_.end()) {
			std::unique_ptr<ObjModel> model = std::make_unique<ObjModel>(objectData.fileName.c_str());
			models_[objectData.fileName] = std::move(model);
		}
	}
}

void JSONLoader::MatUpdate()
{
	for (size_t i = 0; i < objects_.size(); i++)
	{
		objects_[i]->MatUpdate();
	}
}

void JSONLoader::Draw(bool drawShadow)
{
	for (size_t i = 0; i < objects_.size(); i++)
	{
		objects_[i]->Draw(drawShadow);
	}
}
