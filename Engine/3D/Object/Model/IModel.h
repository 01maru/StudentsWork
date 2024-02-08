#pragma once
#include "Material.h"
#include "Mesh.h"
#include <map>

namespace MNE
{

	struct BoneInfo
	{
		MyMath::Matrix boneOffset;
		MyMath::Matrix finalTransformation;
	};

	class IModel
	{
	protected:
		//	boneの最大数
		static const int sMAX_BONE_INDICES = 4;

		//	メッシュ
		std::vector<Mesh> meshes_;
		//	マテリアル
		std::unordered_map<std::string, std::unique_ptr<Material>> materials_;

		//	boneの情報
		MyMath::Matrix globalInverseTransform_;
		std::map<std::string, size_t> boneMapping_;
		size_t numBones_ = 0;
		std::vector<BoneInfo> boneInfo_;

	protected:
		virtual void LoadModel(const std::string& modelname, bool smoothing) = 0;

	public:
		virtual ~IModel() = default;
		//	ファイル名とスムージングするか
		void Initialize(const char* filename, bool smoothing = false);
		virtual void ImGuiUpdate() = 0;
		void Draw(int32_t cBuffMtlIdx);

		virtual void BoneTransform(float timer, std::vector<MyMath::Matrix>& transforms, const std::string& animeName, bool isLoop) = 0;
		
		void AddMaterial(Material* material) { materials_.emplace(material->name_, material); }
		bool SerchMaterial(const std::string& materialName) { return materials_.find(materialName) != materials_.end(); }
		Material* GetMaterial(const std::string& materialName) { return materials_[materialName].get(); }
		//　Getter
		const std::vector<Mesh>& GetMeshes() { return meshes_; }
		const MyMath::Matrix& GetModelTransform() { return globalInverseTransform_; }
		size_t GetNumBones() { return numBones_; }
		std::vector<BoneInfo> GetBoneInfo() { return boneInfo_; }
	};

}
