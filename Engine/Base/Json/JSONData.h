#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Vector3D.h"
#include "Object3D.h"

namespace MNE
{
	struct ModelData
	{
		//	ファイル名
		std::string fileName;
		bool isFbx_ = false;
	};

	struct ObjectData
	{
		std::string fileName;
		//	オブジェクト情報
		MyMath::Vector3D translation;
		MyMath::Vector3D rotation;
		MyMath::Vector3D scaling;
		//	親情報
		ObjectData* parent = nullptr;
	};

	struct CameraData
	{
		MyMath::Vector3D eye;
		int32_t targetID;
		MyMath::Vector3D target;
	};

	struct SpawnPoint
	{
		MyMath::Vector3D pos;
		MyMath::Vector3D rotation;
	};

	class JSONData
	{
	private:
		std::vector<ObjectData> objects_;
		std::map<std::string, CameraData> cameraData_;
		std::vector<SpawnPoint> spawnPoints_;
	public:
		CameraData& GetCameraData(const std::string& name) { return cameraData_[name]; }
		std::map<std::string, CameraData>& GetCameraDatum() { return cameraData_; }
		std::vector<SpawnPoint>& GetSpawnData() { return spawnPoints_; }
		std::vector<ObjectData>& GetObjectData() { return objects_; }

		void SetObjects(std::vector<std::unique_ptr<Object3D>>& objects);
	};

}
